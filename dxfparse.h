#ifndef _DXF_PARSE_H_
#define _DXF_PARSE_H_

#ifdef __cplusplus
extern "C" {
#endif

	int getBoxCount();
	bool dxfparse(char* strFileName,char * Key,char *sErrMsg);
	bool dxfLoadDataFromFile(char *strFileName,char * Key,char *sErrMsg);
	bool dxfLoadDataFromFiles(char strFileName[][512],int filecount,int flag,char *sErrMsg);
	
#ifdef __cplusplus
}

#endif

#endif
