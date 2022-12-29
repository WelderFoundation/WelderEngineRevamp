// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

/// Used to build up shader code in translation. Just wraps some common
/// operations.
class ShaderCodeBuilder
{
public:
  ShaderCodeBuilder();

  // Scope functions are in charge of writing out the scope tokens ('{' and '}')
  // as well as updating the internal scope tracker (for tabs). Ending a class
  // scope also emits the final ';' necessary for a class definition.
  void BeginScope();
  void EndScope();
  void EndClassScope();

  // Often times it is easier to read a complex building of a string using the
  // insertion operator instead of putting all of the statements on another
  // line. For example: builder << returnType << " " << functionName << "(" <<
  // arguments << ")";
  ShaderCodeBuilder& operator<<(StringParam string);
  // To make the Write functions that return ShaderCodeBuilders work we need a
  // special insertion operator that just take in this but doesn't emit
  // anything. However if another builder is passed in we should add all of its
  // contents, hence if the builder passed in is 'this' then we ignore it.
  ShaderCodeBuilder& operator<<(ShaderCodeBuilder& builder);
  ShaderCodeBuilder& operator<<(char value);
  ShaderCodeBuilder& operator<<(Rune value);

  // To work with the insertion operator, functions are needed to emit various
  // special characters.
  String EmitLineReturn();
  String EmitSpace();
  String EmitIndent();

  ShaderCodeBuilder& WriteMemberVariableDeclaration(StringParam variableName, StringParam variableTypeName);
  ShaderCodeBuilder& WriteVariableDeclaration(ShaderIRAttributeList& attributes,
                                              StringParam variableName,
                                              StringParam variableTypeName);
  ShaderCodeBuilder& WriteVariableDeclaration(ShaderIRAttribute& attribute,
                                              StringParam variableName,
                                              StringParam variableTypeName);
  ShaderCodeBuilder& WriteLocalVariableDefaultConstruction(StringParam variableName, StringParam variableTypeName);
  ShaderCodeBuilder& DeclareAttribute(ShaderIRAttribute& attribute);
  ShaderCodeBuilder& DeclareAttribute(StringParam attributeName);
  ShaderCodeBuilder& DeclareAttributeParams(ShaderIRAttribute& attribute);

  // These functions are test replacement functions for the above Emit
  // functions. It's less efficient to build up a string, return it, and then
  // append it into our builder instead of just writing a function to append
  // directly into the builder. To make these work with the insertion operator
  // they return this.
  ShaderCodeBuilder& WriteScopedIndent();

  // Old write functions.
  //@JoshD-ToDo: Clean up later!
  void WriteIndent();
  void WriteIndentation();
  void WriteSpace();
  void Write(StringParam string);
  void WriteLine();
  void WriteLine(StringParam string);

  void Clear();
  String ToString();
  size_t GetSize();

  /// How many scopes are we currently in?
  uint mScopes;
  StringBuilder mBuilder;

  // Cached constant values
  const char* LineReturn;
  const char* Space;
  const char* SingleIndentation;
};

} // namespace Zero
