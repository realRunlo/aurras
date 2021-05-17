#ifndef ___request_h___  
#define ___request_h___


typedef struct request * Request;

Request create_request(char *  id,char ** command);

char * getId(Request req);

char * getCommand(Request req);

char * req_toString(Request req);

Request toReq(char * str);


#endif //___request_h___