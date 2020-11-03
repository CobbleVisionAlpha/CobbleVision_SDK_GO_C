#include <stdbool.h>
#include <cjson/cJSON.h> 

// Functions are all sync. This is due to C requirement to create seperate processes and use other threads for async functions.

// #################################################
// Preparation of Variables and Environment Settings.
// #################################################

bool environmentType = false;
char valid_price_categories=["high", "medium", "low"]
char valid_job_types=["QueuedJob"]

bool debugging = false
char serverAddress = "https://www.cobblevision.com"

char apiUsername = ""
char apiToken = ""

if (environmentType==false || environmentType === "demo"){
  char BaseURL = "https://www.cobblevision.com"
}else{
  char BaseURL = serverAddress + "/api/"
}

// Function allows you to set the Username and Token for CobbleVision
// @function setApiAuth()// @param {String} apiusername
// @param {String} apitoken
// @returns {Boolean} Indicating success of setting Api Auth.

bool setAPIAuth(char ApiUsername, char ApiToken){
  apiUsername = ApiUsername
  apiToken = ApiToken
  return true
}

 // # Function allows you to set the debugging variable
 // # @function setDebugging()
 // # @param {Boolean} debugBool
 // # @returns {Boolean} Indicating success of setting Api Auth.
bool setDebugging(bool debugBool){
  debugging = debugBool
  return true
}

// #####################################################
// # Functions for interacting with the CobbleVision API
// #####################################################

// # Return of the following functions is specified within this type description
// # @typedef {Object} CompletableFuture <HTTPEntity> Entity from URLConnection as Completable Future
// # @method {String} response contains response Data as string


// # This function uploads a media file to CobbleVision. You can find it after login in your media storage. Returns a response object with body, response and headers properties, deducted from npm request module
// # @sync
// # @function uploadMediaFile()  
// # @param {string} price_category - Either high, medium, low
// # @param {boolean} publicBool - Make Media available publicly or not?
// # @param {string} name - Name of Media (Non Unique)
// # @param {array} tags - Tag Names for Media - Array of Strings
// # @param {BufferedImage} file - fileBuffer from file.read
// # @returns {Response} This return the UploadMediaResponse as HttpEntity. The body is in JSON format.
char uploadMediaFile(char price_category, bool publicBool, char name, char tags[], char file[]){
  char endpoint = "media"
  
  if(BaseURL.charAt(sizeOf(BaseURL)-1) != "/"){
    printf("BaseURL must end with slash");
    exit(0)
  }
  
  //Verify the strchar function on char []. usually used on chars, not char [].
  if(!strchar(valid_price_categories,price_category)){
    printf("Job Type is not valid!")
    exit(0)
  }
  
  //According to Web ressources file type checking in c runtime are not possible
  
  cJSON jsonObject = CJSON_CreateObject()
  cJSON_AddStringToObject(jsonObject, "price_Category", price_category)
  cJSON_AddBoolToObject(jsonObject, "publicBool", publicBool)
  cJSON_AddStringToObject(jsonObject, "name", name)
  cJSON_AddArrayToObject(jsonObject, "tags", tags)
  cJSON.AddStringToObject(jsonObject, "file", file)
  
  char response = sendRequestToAPI("POST", jsonObject, BaseURL + endpoint)
  return response
}

// # This function deletes Media from CobbleVision 
// # @sync
// # @function deleteMediaFile()  
// # @param {array} IDArray Array of ID's as Strings
// # @returns {Response} This return the DeleteMediaResponse. The body is in JSON format.

char deleteMediaFile(char IDArray[]){
  char endpoint = "media"
  
  if(BaseURL.charAt(sizeOf(BaseURL)-1) != "/"){
    printf("BaseURL must end with slash");
    exit(0)
  }
  
  bool validResult = checkIDArrayForInvalidIDs(IDArray)
  if(validResult == false){
    printf("Provided Media IDs which are not valid!")
    exit(0)
  }
  
  cJSONArray json_idArray = cJSON_Parse(IDArray)
  char response = sendRequestToAPI("DELETE", null, BaseURL + endpoint + "?id=" + cJSON_Print(json_idArray))
  
  return response
}

// # Launch a calculation with CobbleVision's Web API. Returns a response object with body, response and headers properties, deducted from npm request module;
// # @sync
// # @function launchCalculation() 
// # @param {array} algorithms Array of Algorithm Names
// # @param {array} media Array of Media ID's  
// # @param {string} type Type of Job - Currently Always "QueuedJob"
// # @param {string} [notificationURL] Optional - Notify user upon finishing calculation!
// # @returns {Response} This returns the LaunchCalculationResponse. The body is in JSON format.  

char launchCalculation(char algorithms[], char media[], char type, char notificationURL){
  char endpoint = "calculation"
  
  if(BaseURL.charAt(sizeOf(BaseURL)-1) != "/"){
    printf("BaseURL must end with slash");
    exit(0)
  }
  
  bool validMedia = checkIDArrayForInvalidIDs(media)
  if(validMedia == false){
    printf("Provided Media IDs which are not valid!")
    exit(0)
  }
  
  bool validAlgorithms = checkIDArrayForInvalidIDs(algorithms)
  if(validMedia == false){
    printf("Provided Media IDs which are not valid!")
    exit(0)
  }
  
  //Verify the strchar function on char []. usually used on chars, not char [].
  if(!strchar(valid_job_types,type)){
    printf("Job Type is not valid!")
    exit(0)
  }
  
  cJSON jsonObject = CJSON_CreateObject()
  cJSON_AddArrayToObject(jsonObject, "algorithms", algorithms)
  cJSON_AddArrayToObject(jsonObject, "media", media)
  cJSON_AddStringToObject(jsonObject, "type", type)
  cJSON_AddStringToObject(jsonObject, "notificationURL", notificationURL)
  
  char response = sendRequestToAPI("POST", jsonObject, BaseURL + endpoint)
  
  return response
}

// # This function waits until the given calculation ID's are ready to be downloaded!
// # @sync
// # @function waitForCalculationCompletion() 
// # @param {array} calculationIDArray Array of Calculation ID's
// # @returns {Response} This returns the WaitForCalculationResponse. The body is in JSON format.   

char waitForCalculationCompletion(char IDArray[]){
  char endpoint = "calculation"
  
  if(BaseURL.charAt(sizeOf(BaseURL)-1) != "/"){
    printf("BaseURL must end with slash");
    exit(0)
  }
  
  bool validResult = checkIDArrayForInvalidIDs(IDArray)
  if(validResult == false){
    printf("Provided Calculation IDs which are not valid!")
    exit(0)
  }
  
  cJSONArray json_idArray = cJSON_Parse(IDArray)
  bool calculationFinishedBool = false;
  while(!calculationFinishedBool(){  
    char response = sendRequestToAPI("GET", null, BaseURL + endpoint + "?id=" + cJSON_print(json_idArray), + "&returnOnlyStatusBool=true")
    cJSONArray responseArray = cJSON_Parse(response)
    if(cJSON_IsArray(responseArray)){
      for(int i=0; i<cJSON_GetArraySize(responseArray); i++){
        if(responseArray[i].status){
          if(responseArray[i].status != "finished"){
            calculationFinishedBool = false
          }
        }else{
          calculationFinishedBool = false;
          break;
        }
      }
    }else{
      responseJSON = cJSON_Parse(response)
      if(responseJSON.error){
        calculationFinishedBool = true
      }
    }
  }
  
  return response
}

// # This function deletes Result Files or calculations in status "waiting" from CobbleVision. You cannot delete finished jobs beyond their result files, as we keep them for billing purposes.
// # @sync
// # @function deleteCalculation()
// # @param {array} IDArray Array of ID's as Strings
// # @returns {Response} This returns the DeleteCalculationResponse. The body is in JSON format.

char deleteCalculation(char IDArray[]){
  char endpoint = "calculation"
  
  if(BaseURL.charAt(sizeOf(BaseURL)-1) != "/"){
    printf("BaseURL must end with slash");
    exit(0)
  }
  
  bool validResult = checkIDArrayForInvalidIDs(IDArray)
  if(validResult == false){
    printf("Provided Calculation IDs which are not valid!")
    exit(0)
  }
  
  cJSONArray json_idArray = cJSON_Parse(IDArray)
  char response = sendRequestToAPI("DELETE", null, BaseURL + endpoint + "?id=" + cJSON_Print(json_idArray))
  
  return response
}

// # Get Calculation Result with CobbleVision's Web API. Returns a response object with body, response and headers properties, deducted from npm request module;
// # @sync
// # @function getCalculationResult()
// # @param {array} IDArray ID of calculation to return result Array 
// # @param {boolean} returnOnlyStatusBool Return full result or only status? See Doc for more detailed description!
// # @returns {Response} This returns the GetCalculationResult. The body is in json format.

char getCalculationResult(char IDArray[], bool returnOnlyStatusBool){
  char endpoint = "calculation"
  
  if(BaseURL.charAt(sizeOf(BaseURL)-1) != "/"){
    printf("BaseURL must end with slash");
    exit(0)
  }
  
  bool validResult = checkIDArrayForInvalidIDs(IDArray)
  if(validResult == false){
    printf("Provided Calculation IDs which are not valid!")
    exit(0)
  }
  
  cJSONArray json_idArray = cJSON_Parse(IDArray)
  char response = sendRequestToAPI("GET", null, BaseURL + endpoint + "?id=" + cJSON_Print(json_idArray) + "&returnOnlyStatusBool=" + (char) returnOnlyStatusBool)
  
  return response
}

// # Request your calculation result by ID with the CobbleVision API. Returns a response object with body, response and headers properties, deducted from npm request module;
// # @sync
// # @function getCalculationVisualization()
// # @param {string} id ID of calculation to return result/check String
// # @param {boolean} returnBase64Bool Return Base64 String or image buffer as string?
// # @param {integer} width target width of visualization file
// # @param {integer} height target height of visualization file
// # @returns {Response} This returns the GetCalculationVisualization Result. The body is in binary format.

char getCalculationVisualization(char IDArray[], bool returnBase64Bool){
  char endpoint = "calculation/visualization"
  
  if(BaseURL.charAt(sizeOf(BaseURL)-1) != "/"){
    printf("BaseURL must end with slash");
    exit(0)
  }
  
  bool validResult = checkIDArrayForInvalidIDs(IDArray)
  if(validResult == false){
    printf("Provided Calculation IDs which are not valid!")
    exit(0)
  }
  
  if(width==0){
    printf("Width cannot be zero!")
    exit(0)
  }
  
  if(height==0){
    printf("Height cannot be zero!")
    exit(0)
  }
  
  cJSONArray json_idArray = cJSON_Parse(IDArray)
  char response = sendRequestToAPI("GET", null, BaseURL + endpoint + "?id=" + cJSON_Print(json_idArray) + "&returnBase64Bool=" + (char) returnBase64Bool)
  
  return response
}

// Check Array of Mongo IDs for Invalid Values
// @sync
// @function checkIDArraysForInvalidIDs()
// @param {array} IDArray Array of Mongo IDs
// @returns {boolean} Success of Check

bool checkIDArraysForInvalidIDs(char IDArray[]){
  for(int i=0; i< sizeof(IDArray); i++){
    ObjectId(IDArray[i])
  }
  return true
}

// @sync
// @function wait()
// @param {number} ms time to wait in ms
// @returns {boolean} Success of Wait

bool wait(int ms){
  sleep(ms/1000)
  return true
}



// #####################################################
// # Web Requests in C are a little bit unusual
// #####################################################

// Struct for writeMemoryCallback()
// @struct write_result()
struct write_result{
  char *data;
  pos=0
}

// Callback Function for CURLOPT_WRITEFUNCTION
// @sync
// @function WriteMemoryCallback()
// @param {void} ptr MemoryPointer
// @param {size_t} size Memory Size
// @param {size_t} nmemb Number of elements
// @param {void} stream Response
// @returns {size_t} Memory Size
static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  struct write_result *result = (struct write_result *) stream;
 
  if(result->pos + size * nmemb >= BUFFER_SIZE - 1)
    {
        fprintf(stderr, "error: too small buffer\n");
        return 0;
    }

    memcpy(result->data + result->pos, ptr, size * nmemb);
    result->pos += size * nmemb;

    return size * nmemb;
}

// Functions provides interface for web requests
// @sync
// @function sendRequestToAPI()
// @param {char} type Type of Requests
// @param {cJSON_Object} jsonObject JSON to send with Requests
// @param {char} url Target URL of Requests
// @returns {boolean} Success of Wait
char sendRequestToAPI(char type, cJSON jsonObject, char url){
  #This uses the libcurl library
  
  struct write_result result;
  
  CURL webHandler = curl_easy_init();
  curl_easy_setopt(webHandler, CURLOPT_CUSTOMREQUEST, type)
  curl_easy_setopt(webHandler, CURLOPT_URL, url)
  curl_easy_setopt(webHandler, CURLOPT_WRITEFUNCTION, WriteMemoryCallback)
  
  struct urlList headers = NULL

  headers=curl_slist_append(headers, "Content_Type":"application/json")
  headers_curl_slist_append(headers, "Accept", "application/json")
  curl_easy_setopt(webHandler, CURLOPT_HTTPHEADER, headers)
  curl_easy_setopt(url_handle, CURLOPT_WRITEDATA, result -> data)
  
  if(jsonObject != null && type === "POST"){
    curl_easy_setopt(webHandler, CURLOPT_POSTFIELDS, jsonObject)
    curl_easy_setopt(webHandler, CURLOPT_POSTFIELDSIZE, cJSON_GetArraySize(jsonObject))
  }
  
  status = curl_easy_perform(webHandler)
  
  if(status != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",  
    curl_easy_strerror(res));
  }
        
  result -> data[result.pos] = '\0';
  
  return result -> data;
}



