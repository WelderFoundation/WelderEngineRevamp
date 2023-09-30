// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

// Forward declarations
class BinaryBufferSaver;
class BinaryBufferLoader;
struct ConnectionData;

/// This event can be sent over the network.
class SendableEvent : public Event
{
public:
  ZilchDeclareType(SendableEvent, TypeCopyMode::ReferenceType);

  /// Constructor.
  SendableEvent();

  /// Serialization of the event.
  virtual void Serialize(Serializer& stream);

  /// Because the event could be implemented in scripting, it needs to know how
  /// to destroy itself. The standard method is to call delete on its own this
  /// pointer.
  virtual void Delete();

  /// Write an event to a stream.
  static void Save(SendableEvent* event, Serializer& stream);

  /// Read an event from a stream.
  static SendableEvent* Load(Serializer& stream);

  // Connection data from the socket
  const ConnectionData* Connection;
};

} // namespace Zero
