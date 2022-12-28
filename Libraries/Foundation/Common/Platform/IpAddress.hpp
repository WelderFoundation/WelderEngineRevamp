// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

//                                  IpAddress //
class IpAddress;

void UpdateHostPortString(Status& status, IpAddress& ipAddress);

/// IPv4/IPv6 network host identifier
/// Provided for convenience
/// Note: This class is not slice-able, it has extra data
class ZeroShared IpAddress : public SocketAddress
{
private:
  /// Base methods and data hidden to provide desired behavior
  using SocketAddress::GetIpPort;
  using SocketAddress::mPrivateData;
  using SocketAddress::SetIpPort;
  using SocketAddress::SetIpv4;
  using SocketAddress::SetIpv6;

public:
  /// Creates an empty IP address
  IpAddress();

  /// Creates an IP address identifying the specified IPv4/IPv6 host and port
  /// Will block until host name resolution completes or times out
  IpAddress(Status& status, StringParam host, uint port, InternetProtocol::Enum internetProtocol);
  IpAddress(Status& status, StringParam host, uint port);
  IpAddress(StringParam host, uint port, InternetProtocol::Enum internetProtocol);
  IpAddress(StringParam host, uint port);

  /// Copy Constructors
  IpAddress(const IpAddress& rhs);
  IpAddress(const SocketAddress& rhs);

  // Move Constructor
  IpAddress(MoveReference<IpAddress> rhs);

  /// Copy Assignment Operators
  IpAddress& operator=(const IpAddress& rhs);
  IpAddress& operator=(const SocketAddress& rhs);

  /// Returns true if this is a non-empty IPv4/IPv6 address, else false
  bool IsValid() const;

  /// Returns the valid IP address protocol version, else
  /// InternetProtocol::Unspecified
  InternetProtocol::Enum GetInternetProtocol() const;

  /// Returns the valid IP address as a numeric "host:port" string, else
  /// String()
  const String& GetString() const;
  /// Returns the valid IP address as a hash value, else 0
  size_t Hash() const;

  /// Sets the IP address host
  /// Specifying InternetProtocol::Unspecified will attempt to resolve IPv6
  /// first, then IPv4 Will block until host name resolution completes or times
  /// out
  void SetHost(Status& status, StringParam host, InternetProtocol::Enum internetProtocol);
  void SetHost(Status& status, StringParam host);
  void SetHost(StringParam host, InternetProtocol::Enum internetProtocol);
  void SetHost(StringParam host);
  /// Returns the valid IP address host as a numeric string, else String()
  String GetHost() const;

  /// Sets the valid IP address port
  void SetPort(Status& status, uint port);
  void SetPort(uint port);
  /// Returns the valid IP address port, else 0
  uint GetPort() const;
  /// Returns the valid IP address port as a numeric string, else String()
  String GetPortString() const;

  /// Clears the IP address
  void Clear();

  /// IP address as a numeric "host:port" string
  String mHostPortString;

  // Friends
  template <typename IpAddress>
  friend Bits Serialize(SerializeDirection::Enum direction, BitStream& bitStream, IpAddress& ipAddress);
};

/// Serializes an IP address
/// Returns the number of bits serialized if successful, else 0
template <>
inline ZeroShared Bits Serialize<IpAddress>(SerializeDirection::Enum direction,
                                            BitStream& bitStream,
                                            IpAddress& ipAddress)
{
  // Serialize socket address
  Bits result = Serialize(direction, bitStream, static_cast<SocketAddress&>(ipAddress));

  // Read operation?
  if (direction == SerializeDirection::Read)
  {
    // Update internal host port string
    if (ipAddress.IsValid())
    {
      Status status;
      UpdateHostPortString(status, ipAddress);
      if (status.Failed()) // Unable?
      {
        ipAddress.Clear();
        return 0;
      }
    }
    else
    {
      ipAddress.Clear();
      return 0;
    }
  }

  // Success
  return result;
};

} // namespace Zero
