#pragma once

enum SendPropType
{
    DPT_Int = 0,
    DPT_Float,
    DPT_Vector,
    DPT_VectorXY, // Only encodes the XY of a vector, ignores Z
    DPT_String,
    DPT_Array,     // An array of the base types (can't be of datatables).
    DPT_DataTable,
    DPT_Int64,
    DPT_NUMSendPropTypes
};

struct RecvTable;
class RecvProp;
struct CRecvProxyData;

struct DVariant {

    union {

        float m_Float;

        long m_Int;

        char* m_pString;

        void* m_pData;

        float m_Vector[3];

        __int64 m_Int64;

    };



    int    m_Type;

};


typedef void(__cdecl* RecvVarProxyFn)(const CRecvProxyData*, void*, void*);

struct CRecvProxyData {

    const RecvProp* m_pRecvProp;

    DVariant m_Value;

    int m_iElement;

    int m_ObjectID;

};


struct RecvTable {
public:

    RecvProp* m_pProps;

    int m_nProps;

    void* m_pDecoder;

    char* m_pNetTableName;

private:
    char pad00[2];
};

class RecvProp
{
public:

    char* m_pVarName;
    SendPropType             m_RecvType;
    int                      m_Flags;
    int                      m_StringBufferSize;
    bool                     m_bInsideArray;
    const void* m_pExtraData;
    RecvProp* m_pArrayProp;
    void* m_ArrayLengthProxy;
    RecvVarProxyFn m_ProxyFn;
    void* m_DataTableProxyFn;
    RecvTable* m_pDataTable;
    int                      m_Offset;
    int                      m_ElementStride;
    int                      m_nElements;
    const char* m_pParentArrayPropName;
};