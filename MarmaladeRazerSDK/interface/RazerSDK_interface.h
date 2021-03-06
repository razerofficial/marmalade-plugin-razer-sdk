/*
 * WARNING: this is an autogenerated file and will be overwritten by
 * the extension interface script.
 */
/**
 * Definitions for functions types passed to/from s3eExt interface
 */
typedef        int(*Plugin_getAxis_t)(int deviceId, int axis);
typedef       bool(*Plugin_isPressed_t)(int deviceId, int keyCode);
typedef       bool(*Plugin_isPressedDown_t)(int deviceId, int keyCode);
typedef       bool(*Plugin_isPressedUp_t)(int deviceId, int keyCode);
typedef       void(*Plugin_clearButtonStates_t)();
typedef const char*(*Plugin_getDeviceName_t)(int playerNum);
typedef       void(*Plugin_initPlugin_t)(const char* secretApiKey, s3eCallback onSuccess, s3eCallback onFailure);
typedef       void(*Plugin_requestLogin_t)(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel);
typedef       void(*Plugin_requestGamerInfo_t)(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel);
typedef       void(*Plugin_requestProducts_t)(const char* productsJson, s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel);
typedef       void(*Plugin_requestPurchase_t)(const char* purchasable, const char* productType, s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel);
typedef       void(*Plugin_requestReceipts_t)(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel);
typedef       void(*Plugin_shutdown_t)(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel);
typedef        int(*Plugin_JSONObject_Construct_t)();
typedef       void(*Plugin_JSONObject_Put_t)(int jsonObject, const char* name, const char* value);
typedef const char*(*Plugin_JSONObject_ToString_t)(int jsonObject);
typedef        int(*Plugin_JSONArray_Construct_t)();
typedef       void(*Plugin_JSONArray_Put_t)(int jsonArray, int index, int jsonObject);
typedef       void(*Plugin_JSONArray_PutString_t)(int jsonArray, int index, const char* item);
typedef const char*(*Plugin_JSONArray_ToString_t)(int jsonArray);

/**
 * struct that gets filled in by RazerSDKRegister
 */
typedef struct RazerSDKFuncs
{
    Plugin_getAxis_t m_Plugin_getAxis;
    Plugin_isPressed_t m_Plugin_isPressed;
    Plugin_isPressedDown_t m_Plugin_isPressedDown;
    Plugin_isPressedUp_t m_Plugin_isPressedUp;
    Plugin_clearButtonStates_t m_Plugin_clearButtonStates;
    Plugin_getDeviceName_t m_Plugin_getDeviceName;
    Plugin_initPlugin_t m_Plugin_initPlugin;
    Plugin_requestLogin_t m_Plugin_requestLogin;
    Plugin_requestGamerInfo_t m_Plugin_requestGamerInfo;
    Plugin_requestProducts_t m_Plugin_requestProducts;
    Plugin_requestPurchase_t m_Plugin_requestPurchase;
    Plugin_requestReceipts_t m_Plugin_requestReceipts;
    Plugin_shutdown_t m_Plugin_shutdown;
    Plugin_JSONObject_Construct_t m_Plugin_JSONObject_Construct;
    Plugin_JSONObject_Put_t m_Plugin_JSONObject_Put;
    Plugin_JSONObject_ToString_t m_Plugin_JSONObject_ToString;
    Plugin_JSONArray_Construct_t m_Plugin_JSONArray_Construct;
    Plugin_JSONArray_Put_t m_Plugin_JSONArray_Put;
    Plugin_JSONArray_PutString_t m_Plugin_JSONArray_PutString;
    Plugin_JSONArray_ToString_t m_Plugin_JSONArray_ToString;
} RazerSDKFuncs;
