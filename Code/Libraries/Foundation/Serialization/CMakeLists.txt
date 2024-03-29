add_library(Serialization)

welder_setup_library(Serialization ${CMAKE_CURRENT_LIST_DIR} TRUE)
welder_use_precompiled_header(Serialization ${CMAKE_CURRENT_LIST_DIR})

target_sources(Serialization
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Binary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Binary.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DataTree.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DataTree.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DataTreeNode.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DataTreeNode.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DataTreeParser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DataTreeParser.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DataTreeTokenizer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DataTreeTokenizer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DefaultSerializer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EnumSerialization.hpp
    ${CMAKE_CURRENT_LIST_DIR}/FundamentalTypes.hpp
    ${CMAKE_CURRENT_LIST_DIR}/LegacyDataTreeParser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/LegacyDataTreeParser.hpp
    ${CMAKE_CURRENT_LIST_DIR}/MathSerialization.hpp
    ${CMAKE_CURRENT_LIST_DIR}/MetaSerialization.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MetaSerialization.hpp
    ${CMAKE_CURRENT_LIST_DIR}/OldDataTreeParser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/OldDataTreeParser.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SerializationBuilder.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SerializationStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SerializationStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SerializationTraits.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SerializationUtility.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SerializationUtility.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Simple.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Simple.hpp
    ${CMAKE_CURRENT_LIST_DIR}/StdContainers.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Text.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Text.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Tokenizer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Tokenizer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ZeroContainers.hpp
)

target_link_libraries(Serialization
  PUBLIC
    Common
    Zilch
    Platform
    Meta
)
