/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; c-file-style: "stroustrup" -*-
 *
 * Copyright (C) Opera Software ASA  2002 - 2006
 *
 * Table of instruction data.
 *
 * @author modules/ecmascript/carakan/src/scripts/es_instruction_data.h.py
 */

#ifndef ES_INSTRUCTION_DATA_H
#define ES_INSTRUCTION_DATA_H

const unsigned g_instruction_operand_count[] =
{
    2,        // ESI_LOAD_STRING
    2,        // ESI_LOAD_DOUBLE
    2,        // ESI_LOAD_INT32
    1,        // ESI_LOAD_NULL
    1,        // ESI_LOAD_UNDEFINED
    1,        // ESI_LOAD_TRUE
    1,        // ESI_LOAD_FALSE
    1,        // ESI_LOAD_GLOBAL_OBJECT
    2,        // ESI_COPY
    UINT_MAX, // ESI_COPYN
    2,        // ESI_TYPEOF
    2,        // ESI_TONUMBER
    2,        // ESI_TOOBJECT
    3,        // ESI_TOPRIMITIVE
    2,        // ESI_TOPRIMITIVE1
    1,        // ESI_IS_NULL_OR_UNDEFINED
    1,        // ESI_IS_NOT_NULL_OR_UNDEFINED
    3,        // ESI_ADD
    3,        // ESI_ADD_IMM
#ifdef ES_COMBINED_ADD_SUPPORT
    UINT_MAX, // ESI_ADDN
#endif
    5,        // ESI_FORMAT_STRING
    3,        // ESI_SUB
    3,        // ESI_SUB_IMM
    3,        // ESI_MUL
    3,        // ESI_MUL_IMM
    3,        // ESI_DIV
    3,        // ESI_DIV_IMM
    3,        // ESI_REM
    3,        // ESI_REM_IMM
    3,        // ESI_LSHIFT
    3,        // ESI_LSHIFT_IMM
    3,        // ESI_RSHIFT_SIGNED
    3,        // ESI_RSHIFT_SIGNED_IMM
    3,        // ESI_RSHIFT_UNSIGNED
    3,        // ESI_RSHIFT_UNSIGNED_IMM
    2,        // ESI_NEG
    2,        // ESI_COMPL
    1,        // ESI_INC
    1,        // ESI_DEC
    3,        // ESI_BITAND
    3,        // ESI_BITAND_IMM
    3,        // ESI_BITOR
    3,        // ESI_BITOR_IMM
    3,        // ESI_BITXOR
    3,        // ESI_BITXOR_IMM
    2,        // ESI_NOT
    2,        // ESI_EQ
    2,        // ESI_EQ_IMM
    2,        // ESI_NEQ
    2,        // ESI_NEQ_IMM
    2,        // ESI_EQ_STRICT
    2,        // ESI_EQ_STRICT_IMM
    2,        // ESI_NEQ_STRICT
    2,        // ESI_NEQ_STRICT_IMM
    2,        // ESI_LT
    2,        // ESI_LT_IMM
    2,        // ESI_LTE
    2,        // ESI_LTE_IMM
    2,        // ESI_GT
    2,        // ESI_GT_IMM
    2,        // ESI_GTE
    2,        // ESI_GTE_IMM
    1,        // ESI_CONDITION
    1,        // ESI_JUMP
    1,        // ESI_JUMP_INDIRECT
    2,        // ESI_JUMP_IF_TRUE
    2,        // ESI_JUMP_IF_FALSE
    1,        // ESI_START_LOOP
    1,        // ESI_STORE_BOOLEAN
    4,        // ESI_GETN_IMM
    4,        // ESI_PUTN_IMM
    4,        // ESI_INIT_PROPERTY
    4,        // ESI_GET_LENGTH
    4,        // ESI_PUT_LENGTH
    3,        // ESI_GETI_IMM
    3,        // ESI_PUTI_IMM
    3,        // ESI_GET
    3,        // ESI_PUT
    4,        // ESI_DEFINE_GETTER
    4,        // ESI_DEFINE_SETTER
    5,        // ESI_GET_SCOPE
    6,        // ESI_GET_SCOPE_REF
    4,        // ESI_PUT_SCOPE
    3,        // ESI_DELETE_SCOPE
    3,        // ESI_GET_GLOBAL
    3,        // ESI_PUT_GLOBAL
    3,        // ESI_GET_LEXICAL
    3,        // ESI_PUT_LEXICAL
    2,        // ESI_DELETEN_IMM
    2,        // ESI_DELETEI_IMM
    2,        // ESI_DELETE
    1,        // ESI_DECLARE_GLOBAL
    2,        // ESI_HASPROPERTY
    2,        // ESI_INSTANCEOF
    3,        // ESI_ENUMERATE
    4,        // ESI_NEXT_PROPERTY
    4,        // ESI_EVAL
    2,        // ESI_CALL
    2,        // ESI_REDIRECTED_CALL
    2,        // ESI_APPLY
    2,        // ESI_CONSTRUCT
    1,        // ESI_RETURN_VALUE
    0,        // ESI_RETURN_NO_VALUE
    1,        // ESI_RETURN_FROM_EVAL
    2,        // ESI_NEW_OBJECT
    UINT_MAX, // ESI_CONSTRUCT_OBJECT
    2,        // ESI_NEW_ARRAY
    3,        // ESI_CONSTRUCT_ARRAY
    3,        // ESI_NEW_FUNCTION
    2,        // ESI_NEW_REGEXP
    2,        // ESI_TABLE_SWITCH
    1,        // ESI_CATCH
    2,        // ESI_CATCH_SCOPE
    1,        // ESI_THROW
    1,        // ESI_THROW_BUILTIN
    3,        // ESI_RETHROW
    0,        // ESI_EXIT
    0,        // ESI_EXIT_TO_BUILTIN
    0,        // ESI_EXIT_TO_EVAL
#ifdef ECMASCRIPT_DEBUGGER
    1,        // ESI_DEBUGGER_STOP
#endif
    0
};

const unsigned short g_instruction_operand_register_io[] =
{
    0x0001, // ESI_LOAD_STRING
    0x0001, // ESI_LOAD_DOUBLE
    0x0001, // ESI_LOAD_INT32
    0x0001, // ESI_LOAD_NULL
    0x0001, // ESI_LOAD_UNDEFINED
    0x0001, // ESI_LOAD_TRUE
    0x0001, // ESI_LOAD_FALSE
    0x0001, // ESI_LOAD_GLOBAL_OBJECT
    0x0201, // ESI_COPY
    0x0000, // ESI_COPYN
    0x0201, // ESI_TYPEOF
    0x0201, // ESI_TONUMBER
    0x0201, // ESI_TOOBJECT
    0x0201, // ESI_TOPRIMITIVE
    0x0101, // ESI_TOPRIMITIVE1
    0x0100, // ESI_IS_NULL_OR_UNDEFINED
    0x0100, // ESI_IS_NOT_NULL_OR_UNDEFINED
    0x0601, // ESI_ADD
    0x0201, // ESI_ADD_IMM
#ifdef ES_COMBINED_ADD_SUPPORT
    0x0000, // ESI_ADDN
#endif
    0x0201, // ESI_FORMAT_STRING
    0x0601, // ESI_SUB
    0x0201, // ESI_SUB_IMM
    0x0601, // ESI_MUL
    0x0201, // ESI_MUL_IMM
    0x0601, // ESI_DIV
    0x0201, // ESI_DIV_IMM
    0x0601, // ESI_REM
    0x0201, // ESI_REM_IMM
    0x0601, // ESI_LSHIFT
    0x0201, // ESI_LSHIFT_IMM
    0x0601, // ESI_RSHIFT_SIGNED
    0x0201, // ESI_RSHIFT_SIGNED_IMM
    0x0601, // ESI_RSHIFT_UNSIGNED
    0x0201, // ESI_RSHIFT_UNSIGNED_IMM
    0x0201, // ESI_NEG
    0x0201, // ESI_COMPL
    0x0101, // ESI_INC
    0x0101, // ESI_DEC
    0x0601, // ESI_BITAND
    0x0201, // ESI_BITAND_IMM
    0x0601, // ESI_BITOR
    0x0201, // ESI_BITOR_IMM
    0x0601, // ESI_BITXOR
    0x0201, // ESI_BITXOR_IMM
    0x0201, // ESI_NOT
    0x0300, // ESI_EQ
    0x0100, // ESI_EQ_IMM
    0x0300, // ESI_NEQ
    0x0100, // ESI_NEQ_IMM
    0x0300, // ESI_EQ_STRICT
    0x0100, // ESI_EQ_STRICT_IMM
    0x0300, // ESI_NEQ_STRICT
    0x0100, // ESI_NEQ_STRICT_IMM
    0x0300, // ESI_LT
    0x0100, // ESI_LT_IMM
    0x0300, // ESI_LTE
    0x0100, // ESI_LTE_IMM
    0x0300, // ESI_GT
    0x0100, // ESI_GT_IMM
    0x0300, // ESI_GTE
    0x0100, // ESI_GTE_IMM
    0x0101, // ESI_CONDITION
    0x0000, // ESI_JUMP
    0x0100, // ESI_JUMP_INDIRECT
    0x0000, // ESI_JUMP_IF_TRUE
    0x0000, // ESI_JUMP_IF_FALSE
    0x0000, // ESI_START_LOOP
    0x0001, // ESI_STORE_BOOLEAN
    0x0201, // ESI_GETN_IMM
    0x0500, // ESI_PUTN_IMM
    0x0500, // ESI_INIT_PROPERTY
    0x0201, // ESI_GET_LENGTH
    0x0500, // ESI_PUT_LENGTH
    0x0201, // ESI_GETI_IMM
    0x0500, // ESI_PUTI_IMM
    0x0601, // ESI_GET
    0x0700, // ESI_PUT
    0x0100, // ESI_DEFINE_GETTER
    0x0100, // ESI_DEFINE_SETTER
    0x0801, // ESI_GET_SCOPE
    0x1003, // ESI_GET_SCOPE_REF
    0x0208, // ESI_PUT_SCOPE
    0x0400, // ESI_DELETE_SCOPE
    0x0001, // ESI_GET_GLOBAL
    0x0200, // ESI_PUT_GLOBAL
    0x0001, // ESI_GET_LEXICAL
    0x0400, // ESI_PUT_LEXICAL
    0x0100, // ESI_DELETEN_IMM
    0x0100, // ESI_DELETEI_IMM
    0x0300, // ESI_DELETE
    0x0000, // ESI_DECLARE_GLOBAL
    0x0300, // ESI_HASPROPERTY
    0x0300, // ESI_INSTANCEOF
    0x0207, // ESI_ENUMERATE
    0x0e09, // ESI_NEXT_PROPERTY
    0x0000, // ESI_EVAL
    0x0000, // ESI_CALL
    0x0300, // ESI_REDIRECTED_CALL
    0x0101, // ESI_APPLY
    0x0000, // ESI_CONSTRUCT
    0x0100, // ESI_RETURN_VALUE
    0x0000, // ESI_RETURN_NO_VALUE
    0x0100, // ESI_RETURN_FROM_EVAL
    0x0001, // ESI_NEW_OBJECT
    0x0000, // ESI_CONSTRUCT_OBJECT
    0x0001, // ESI_NEW_ARRAY
    0x0001, // ESI_CONSTRUCT_ARRAY
    0x0001, // ESI_NEW_FUNCTION
    0x0001, // ESI_NEW_REGEXP
    0x0100, // ESI_TABLE_SWITCH
    0x0001, // ESI_CATCH
    0x0001, // ESI_CATCH_SCOPE
    0x0100, // ESI_THROW
    0x0000, // ESI_THROW_BUILTIN
    0x0304, // ESI_RETHROW
    0x0000, // ESI_EXIT
    0x0000, // ESI_EXIT_TO_BUILTIN
    0x0000, // ESI_EXIT_TO_EVAL
#ifdef ECMASCRIPT_DEBUGGER
    0x0000, // ESI_DEBUGGER_STOP
#endif
    0
};

const BOOL g_instruction_is_trivial[] =
{
    TRUE, // ESI_LOAD_STRING
    TRUE, // ESI_LOAD_DOUBLE
    TRUE, // ESI_LOAD_INT32
    TRUE, // ESI_LOAD_NULL
    TRUE, // ESI_LOAD_UNDEFINED
    TRUE, // ESI_LOAD_TRUE
    TRUE, // ESI_LOAD_FALSE
    TRUE, // ESI_LOAD_GLOBAL_OBJECT
    TRUE, // ESI_COPY
    TRUE, // ESI_COPYN
    TRUE, // ESI_TYPEOF
    FALSE, // ESI_TONUMBER
    FALSE, // ESI_TOOBJECT
    FALSE, // ESI_TOPRIMITIVE
    FALSE, // ESI_TOPRIMITIVE1
    FALSE, // ESI_IS_NULL_OR_UNDEFINED
    FALSE, // ESI_IS_NOT_NULL_OR_UNDEFINED
    FALSE, // ESI_ADD
    FALSE, // ESI_ADD_IMM
#ifdef ES_COMBINED_ADD_SUPPORT
    FALSE, // ESI_ADDN
#endif
    FALSE, // ESI_FORMAT_STRING
    FALSE, // ESI_SUB
    FALSE, // ESI_SUB_IMM
    FALSE, // ESI_MUL
    FALSE, // ESI_MUL_IMM
    FALSE, // ESI_DIV
    FALSE, // ESI_DIV_IMM
    FALSE, // ESI_REM
    FALSE, // ESI_REM_IMM
    FALSE, // ESI_LSHIFT
    FALSE, // ESI_LSHIFT_IMM
    FALSE, // ESI_RSHIFT_SIGNED
    FALSE, // ESI_RSHIFT_SIGNED_IMM
    FALSE, // ESI_RSHIFT_UNSIGNED
    FALSE, // ESI_RSHIFT_UNSIGNED_IMM
    FALSE, // ESI_NEG
    FALSE, // ESI_COMPL
    FALSE, // ESI_INC
    FALSE, // ESI_DEC
    FALSE, // ESI_BITAND
    FALSE, // ESI_BITAND_IMM
    FALSE, // ESI_BITOR
    FALSE, // ESI_BITOR_IMM
    FALSE, // ESI_BITXOR
    FALSE, // ESI_BITXOR_IMM
    FALSE, // ESI_NOT
    FALSE, // ESI_EQ
    FALSE, // ESI_EQ_IMM
    FALSE, // ESI_NEQ
    FALSE, // ESI_NEQ_IMM
    TRUE, // ESI_EQ_STRICT
    TRUE, // ESI_EQ_STRICT_IMM
    TRUE, // ESI_NEQ_STRICT
    TRUE, // ESI_NEQ_STRICT_IMM
    FALSE, // ESI_LT
    FALSE, // ESI_LT_IMM
    FALSE, // ESI_LTE
    FALSE, // ESI_LTE_IMM
    FALSE, // ESI_GT
    FALSE, // ESI_GT_IMM
    FALSE, // ESI_GTE
    FALSE, // ESI_GTE_IMM
    TRUE, // ESI_CONDITION
    TRUE, // ESI_JUMP
    TRUE, // ESI_JUMP_INDIRECT
    TRUE, // ESI_JUMP_IF_TRUE
    TRUE, // ESI_JUMP_IF_FALSE
    TRUE, // ESI_START_LOOP
    TRUE, // ESI_STORE_BOOLEAN
    FALSE, // ESI_GETN_IMM
    FALSE, // ESI_PUTN_IMM
    FALSE, // ESI_INIT_PROPERTY
    FALSE, // ESI_GET_LENGTH
    FALSE, // ESI_PUT_LENGTH
    FALSE, // ESI_GETI_IMM
    FALSE, // ESI_PUTI_IMM
    FALSE, // ESI_GET
    FALSE, // ESI_PUT
    FALSE, // ESI_DEFINE_GETTER
    FALSE, // ESI_DEFINE_SETTER
    FALSE, // ESI_GET_SCOPE
    FALSE, // ESI_GET_SCOPE_REF
    FALSE, // ESI_PUT_SCOPE
    FALSE, // ESI_DELETE_SCOPE
    FALSE, // ESI_GET_GLOBAL
    FALSE, // ESI_PUT_GLOBAL
    FALSE, // ESI_GET_LEXICAL
    FALSE, // ESI_PUT_LEXICAL
    FALSE, // ESI_DELETEN_IMM
    FALSE, // ESI_DELETEI_IMM
    FALSE, // ESI_DELETE
    TRUE, // ESI_DECLARE_GLOBAL
    FALSE, // ESI_HASPROPERTY
    FALSE, // ESI_INSTANCEOF
    FALSE, // ESI_ENUMERATE
    FALSE, // ESI_NEXT_PROPERTY
    FALSE, // ESI_EVAL
    FALSE, // ESI_CALL
    FALSE, // ESI_REDIRECTED_CALL
    FALSE, // ESI_APPLY
    FALSE, // ESI_CONSTRUCT
    TRUE, // ESI_RETURN_VALUE
    TRUE, // ESI_RETURN_NO_VALUE
    TRUE, // ESI_RETURN_FROM_EVAL
    TRUE, // ESI_NEW_OBJECT
    TRUE, // ESI_CONSTRUCT_OBJECT
    TRUE, // ESI_NEW_ARRAY
    TRUE, // ESI_CONSTRUCT_ARRAY
    TRUE, // ESI_NEW_FUNCTION
    TRUE, // ESI_NEW_REGEXP
    FALSE, // ESI_TABLE_SWITCH
    TRUE, // ESI_CATCH
    TRUE, // ESI_CATCH_SCOPE
    FALSE, // ESI_THROW
    FALSE, // ESI_THROW_BUILTIN
    FALSE, // ESI_RETHROW
    TRUE, // ESI_EXIT
    TRUE, // ESI_EXIT_TO_BUILTIN
    TRUE, // ESI_EXIT_TO_EVAL
#ifdef ECMASCRIPT_DEBUGGER
    FALSE, // ESI_DEBUGGER_STOP
#endif
    FALSE
};

#endif // ES_INSTRUCTION_DATA_H
