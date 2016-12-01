// dxfReaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dxfReader.h"
#include "dxfReaderDlg.h"

#include <winsock2.h>
#include "mysql.h" 
#include "dxfparse.h"
#include <vector>
#include "writelogdata.h"

#pragma comment(lib, "libmysql.lib")
using namespace std; 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_COMMAND_LEN 1024

/////////////////////////////////////////////////////////////////////////////
// CDxfReaderDlg dialog
vector<string> filenames;

CTime sysTime;

char szCommand[MAX_COMMAND_LEN];

bool WriteMySqlByScript(char * configname,char * key,char* filename,char* strError);

void delquote(char* instr)
{
	char* p = NULL;
	p = strstr(instr,"\"");
	if (p)
		strcpy(instr,++p);
	p = strrchr(instr,'\"');
	if (p)
		*p = '\0';
}

bool splitstr(char *str,char *delims,vector<string>& out)
{
	char *result = NULL;

	result = strtok( str, delims );
	if (result)
		delquote(result);

	while( result != NULL )
	{
		delquote(result);
		out.push_back(result);
		//printf( "result is \"%s\"\n", result );
		result = strtok( NULL, delims );
	}

	if (out.size() > 0)
		return true;
	else
		return false;
}

int gettextdata(char* szErrMsg){
	char szTest[1000] = {0};  
	int count = 0;  

	FILE *fp = fopen("c:\\boxtmp.txt", "r");  
	if(NULL == fp)  
	{  
		strcpy(szErrMsg,"failed to open boxtmp.txt\n");  
		return -1;  
	}  

	while(!feof(fp))  
	{  
		memset(szTest, 0, sizeof(szTest));  
		fgets(szTest, sizeof(szTest) - 1, fp); // 包含了\n 
		if (strlen(szTest) == 0){
			fclose(fp);
			return count;
		}
		count++;
	}  
	fclose(fp);  

	return count;
}

void addFileText(FILE* fw,char* name = "")
{
	char path[255];
	char* ptr;
	int i=0;
	char ch;

	if (!fw)
		return;

	GetModuleFileName(NULL,path,sizeof(path));

	ptr = strrchr(path,'\\');
	if (!ptr)
		return;

	*ptr = '\0';
	if (strlen(name) == 0)
		strcat(path,"\\testcad.sql");
	else
		strcat(path,name);

	FILE *fileR;
	if((fileR = fopen(path,"r")) == NULL)
		return;

	ch =fgetc(fileR);
	while(ch != EOF)
	{
		//buf[i++]=ch;
		fwrite(&ch, 1, 1, fw);
		ch=fgetc(fileR);
	}
	fclose(fileR);
}



void createsql(FILE* fp,vector<string>& data){
	//写入数据库,wtype规定写入的类型
	if (!fp)
		return;
	memset(szCommand,0x00,MAX_COMMAND_LEN);

	char wtype = (data[0])[0];

	switch (wtype)
	{
	case 'L':
		sprintf(szCommand,"insert into ad_line() values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",data[2].c_str(),
			data[3].c_str(),data[4].c_str(),data[5].c_str(),data[6].c_str(),data[7].c_str(),data[8].c_str()
			,data[9].c_str(),data[1].c_str(),data[10].c_str());
		break;
	/*case 'C':
		sprintf(szCommand,"insert into ad_circle() values('%s','%s','%s','%s','%s','%s','%s','%s')",data[2].c_str(),
			data[3].c_str(),data[4].c_str(),data[5].c_str(),data[6].c_str()
			,data[7].c_str(),data[1].c_str(),data[8].c_str());
		sprintf(szCommand,"insert into ad_circle() values('%s','%s','%s','%lf','%s','%s','%d','%s')",circles[iloop].enttype,
			convertToString(circles[iloop].Point0.x).c_str(),convertToString(circles[iloop].Point0.y).c_str(),circles[iloop].Point0.z,
			convertToString(circles[iloop].Radius).c_str(),keyval,frame_id,namew);
		break;
	case 'E':
		sprintf(szCommand,"insert into ad_line() values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",data[2].c_str(),
			data[3].c_str(),data[4].c_str(),data[5].c_str(),data[6].c_str(),data[7].c_str(),data[8].c_str()
			,data[9].c_str(),data[1].c_str(),data[10].c_str());
		sprintf(szCommand,"insert into ad_ellipse() values('%s','%s','%s','%lf','%s','%s','%lf','%s','%s','%s','%s','%d','%s')",ellipses[iloop].enttype,
			convertToString(ellipses[iloop].Point0.x).c_str(),convertToString(ellipses[iloop].Point0.y).c_str(),ellipses[iloop].Point0.z,
			convertToString(ellipses[iloop].Point1.x).c_str(),convertToString(ellipses[iloop].Point1.y).c_str(),ellipses[iloop].Point1.z,
			convertToString(ellipses[iloop].Ratio).c_str(),convertToString(ellipses[iloop].StartAngle).c_str(),
			convertToString(ellipses[iloop].EndAngle).c_str(),keyval,frame_id,namew);
		break;
	case 'A':
		sprintf(szCommand,"insert into ad_arc() values('%s','%s','%s','%lf','%s','%s','%s','%s','%d','%s')",arcs[iloop].enttype,
			convertToString(arcs[iloop].Point0.x).c_str(),convertToString(arcs[iloop].Point0.y).c_str(),arcs[iloop].Point0.z,
			convertToString(arcs[iloop].Radius).c_str(),convertToString(arcs[iloop].StartAngle).c_str(),
			convertToString(arcs[iloop].EndAngle).c_str(),keyval,frame_id,namew);
		break;
	case 'T':
		sprintf(szCommand,"insert into ad_text() values('%s','%s','%s','%lf','%s','%s','%lf','%s','%s','%s','%s','%d','%s')",texts[iloop].enttype,
			convertToString(texts[iloop].Point0.x).c_str(),convertToString(texts[iloop].Point0.y).c_str(),texts[iloop].Point0.z,
			convertToString(texts[iloop].PointS.x).c_str(),convertToString(texts[iloop].PointS.y).c_str(),texts[iloop].PointS.z,
			convertToString(texts[iloop].height).c_str(),convertToString(texts[iloop].rotationangle).c_str(),
			texts[iloop].szText,keyval,frame_id,namew);
		break;*/
	default:
		break;
	}
	if (strlen(szCommand)>0)
	{
		strcat(szCommand,";");
		writeFile(fp,szCommand,0);
	}
}

int txt2sql_wdb(FILE* fw,char* fName,char* szErrMsg){
	char szTest[1000] = {0};  
	int count = 0;  
	
	vector<string> vstr;

	if (!fw){
		strcpy(szErrMsg,"sql file open fail");
		return -1;
	}

	FILE *fp = fopen(fName, "r");  
	if(NULL == fp)  
	{  
		strcpy(szErrMsg,"failed to open boxtmp.txt\n");  
		return -1;  
	}  

	while(!feof(fp))  
	{  
		vector<string>().swap(vstr);
		memset(szTest, 0, sizeof(szTest));  
		fgets(szTest, sizeof(szTest) - 1, fp); // 包含了\n 
		if (strlen(szTest) == 0){
			fclose(fp);
			return 0;
		}
		if(splitstr(szTest,"	",vstr))
			createsql(fw,vstr);
	}  
	fclose(fp);  

	return 0;
}

bool WriteMySqlByScript(char * configname,char * key,char* filename,char* strError)
{
	FILE *fileR;
	char buff[1024];  
	char host[128] = {0};
	char user[128] = {0};
	char passwd[128] = {0};
	char db[128] = {0};
	unsigned int port = 0;

	::GetPrivateProfileString(key,"host","127.0.0.1",host,128,configname);
	::GetPrivateProfileString(key,"user","root",user,128,configname);
	::GetPrivateProfileString(key,"passwd","",passwd,128,configname);
	::GetPrivateProfileString(key,"db","",db,128,configname);
	port = GetPrivateProfileInt(key, "port",3306, configname);

	MYSQL mysql;
	mysql_init(&mysql); // 初始化
	MYSQL *ConnStatus = mysql_real_connect(&mysql,host,user,passwd,db,port,0,0); 
	if (ConnStatus == NULL) { 
		// 连接失败 
		int i = mysql_errno(&mysql); 
		strcpy(strError,mysql_error(&mysql));   
		return false; 
	}   

	MYSQL_RES *result=NULL; // 数据结果集  // 插入操作 
	mysql_query(&mysql,"START TRANSACTION"); // 开启事务， 如果没有开启事务，那么效率会变得非常低下！  

	printf(filename);
	printf("\n");
	if((fileR = fopen(filename,"r")) == NULL){
		strcpy(strError,"open file fail");
		return false;
	}

	while (!feof(fileR)) 
	{ 
		memset(buff,0x00,1024);
		fgets(buff,1024,fileR);  //读取一行
		if (int(buff[strlen(buff)-1]&0xff) == 0x0a)
		{
			buff[strlen(buff)-1] = '\0';
			if(strlen(buff) == 0)
				continue;
		}
		mysql_set_character_set(&mysql, "utf8");
		mysql_query(&mysql,buff); 
	} 

	fclose(fileR);

	mysql_query(&mysql,"COMMIT");   // 提交事务  
	//cout<<"insert end"<<endl;    //释放结果集 关闭数据库 
	mysql_free_result(result); 
	mysql_close(&mysql); 
	mysql_library_end();  

	return true;
}


void GetAllFiles(CString strDirPath,bool del = false)  
{  
	CFileFind FileFinder;  
	CTime tempTime;

	strDirPath  = strDirPath + _T("\\");  
	strDirPath += "*.*";

	BOOL ret = FileFinder.FindFile(strDirPath);
	while (ret)  
	{  
		ret = FileFinder.FindNextFile();
		if (ret != 0)
		{
			//if (!FileFinder.IsDirectory() && !FileFinder.IsDots())
			{
				//CString name = FileFinder.GetFileName();
				CString path = FileFinder.GetFilePath();
				CString fileext = path.Right(4);

				if (fileext == ".dxf")
				{
					if (!del)
					{
						filenames.push_back(path.GetBuffer(0));
					}
					if (FileFinder.GetCreationTime(tempTime))
					{
						//CString str = tempTime.Format(_T("%c"));
						if (tempTime>sysTime && del)
						{
							remove(path);
						}
					}
				}
			}
		}
	}  

}

void FileSearch(CString root,bool del = false)
{ // root 为目录名
	CFileFind ff;
	CString FilePath;
	CTime tempTime;

	if (root.Right(1)!="/")
	{
		root+="/";
	}
	root+="*.*";
	BOOL res=ff.FindFile(root);
	while (res)
	{
		res=ff.FindNextFile();
		FilePath=ff.GetFilePath();
		if (ff.IsDirectory() && !ff.IsDots())// 找到的是文件夹
		{
			//FileSearch(FilePath);// 递归
		}
		else if (!ff.IsDirectory() && !ff.IsDots())// 找到的是文件
		{
			CString m_ff =FilePath;
			if (m_ff.Right(4) == ".dxf")
			{
				if (!del)
				{
					filenames.push_back(m_ff.GetBuffer(0));
				}
				else{
					if (ff.GetCreationTime(tempTime))
					{
						//CString str = tempTime.Format(_T("%c"));
						if (tempTime>sysTime)
						{
							remove(m_ff);
						}
					}
				}
			}
		}
	}
}

CDxfReaderDlg::CDxfReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDxfReaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDxfReaderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDxfReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDxfReaderDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDxfReaderDlg, CDialog)
	//{{AFX_MSG_MAP(CDxfReaderDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &CDxfReaderDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDxfReaderDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDxfReaderDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDxfReaderDlg message handlers

BOOL CDxfReaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDxfReaderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDxfReaderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDxfReaderDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szBuffer[MAX_PATH] = {0}; 
	char szerr[512] = {0};
	OPENFILENAME ofn= {0}; 

	FILE *fileR;
	char buff[1024];  
	char fileext[10] = {0};
	char convfilename[512] = {0};

	char host[128] = {0};
	char user[128] = {0};
	char passwd[128] = {0};
	char db[128] = {0};

	int itype = 0;
	int isTest = 0;
	int ver  = 0;
	int funcno = 0;
	int hide = 0;

	(GetDlgItem(IDC_EDIT1))->SetWindowText("");

	ofn.lStructSize = sizeof(ofn); 
	ofn.hwndOwner = m_hWnd; 
	ofn.lpstrFilter = _T("DWG文件(*.dwg)\0*.dwg\0DXF文件(*.dxf)\0*.dxf\0所有文件(*.*)\0*.*\0");//要选择的文件后缀 
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区 
	ofn.nMaxFile = sizeof(szBuffer)/sizeof(*szBuffer); 
	ofn.nFilterIndex = 0; 
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER ;//标志如果是多选要加上OFN_ALLOWMULTISELECT
	BOOL bSel = GetOpenFileName(&ofn); 

	CString sPath;
	GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer ();
	int nPos;
	nPos=sPath.ReverseFind ('\\');
	sPath=sPath.Left (nPos);
	CString lpszSet = sPath + "\\setting.ini";
	CString lpAppConvPath = sPath + "\\dd.exe";

	isTest = ::GetPrivateProfileInt("General","isTest",0,(LPSTR)(LPCTSTR)lpszSet);
	funcno = ::GetPrivateProfileInt("General","funcno",0,(LPSTR)(LPCTSTR)lpszSet);
	ver = ::GetPrivateProfileInt("General","ver",0,(LPSTR)(LPCTSTR)lpszSet);
	hide = ::GetPrivateProfileInt("General","hide",0,(LPSTR)(LPCTSTR)lpszSet);
	itype = ::GetPrivateProfileInt("General","type",0,(LPSTR)(LPCTSTR)lpszSet);

	if(strlen(szBuffer)>4){
		memset(fileext,0x00,10);
		memcpy(fileext,szBuffer+strlen(szBuffer)-4,4);
		if(strcmp(fileext,".dwg") == 0 || strcmp(fileext,".DWG") == 0){

			memset(convfilename,0x00,512);
			memcpy(convfilename,szBuffer,strlen(szBuffer)-4);
			strcat(convfilename,".dxf");

			SHELLEXECUTEINFO ShExecInfo = {0};
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = NULL;
			ShExecInfo.lpFile = lpAppConvPath;
			memset(buff,0x00,1024);

			if (hide == 0)
			{
				if (ver == 1)
					sprintf(buff,"/InFile %s /OutFile %s /OutVer acad12 /Hide",szBuffer,convfilename);
				if (ver == 0)
					sprintf(buff,"/InFile %s /OutFile %s /OutVer acad2007 /Hide",szBuffer,convfilename);
			}
			if (hide == 1)
			{
				if (ver == 1)
					sprintf(buff,"/InFile %s /OutFile %s /OutVer acad12",szBuffer,convfilename);
				if (ver == 0)
					sprintf(buff,"/InFile %s /OutFile %s /OutVer acad2007",szBuffer,convfilename);
			}

			ShExecInfo.lpParameters = buff;
			ShExecInfo.lpDirectory = NULL;
			ShExecInfo.nShow = SW_SHOW;
			ShExecInfo.hInstApp = NULL;
			if (!ShellExecuteEx(&ShExecInfo))
			{
				MessageBox("can not execute dd.exe","INFO",0);
				return;
			}
			else{
				WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
			}

			memset(buff,0x00,1024);
			bool rt = false;
			if (funcno == 0)
				rt = dxfparse(convfilename,"123456",szerr);
			if (funcno == 1)
				rt = dxfLoadDataFromFile(convfilename,"123456",szerr);
			if (rt)
			{
				(GetDlgItem(IDC_EDIT1))->SetWindowText("parse ok");
				if (!isTest)
					remove(convfilename);
			}
			else{
				(GetDlgItem(IDC_EDIT1))->SetWindowText(szerr);
				return;
			}
		}
		else if(strcmp(fileext,".dxf") == 0 || strcmp(fileext,".DXF") == 0)
		{
			bool rt = false;// = dxfparse(szBuffer,"test",szerr);
			if (funcno == 0)
				rt = dxfparse(szBuffer,"123456",szerr);
			if (funcno == 1)
				rt = dxfLoadDataFromFile(szBuffer,"123456",szerr);

			if (rt)
			{
				(GetDlgItem(IDC_EDIT1))->SetWindowText("parse ok");
			}
			else{
				(GetDlgItem(IDC_EDIT1))->SetWindowText(szerr);
				return;
			}
		}
		else
		{
			(GetDlgItem(IDC_EDIT1))->SetWindowText("file format error");
			return;
		}
	}
	else
	{
		(GetDlgItem(IDC_EDIT1))->SetWindowText("file format error");
		return;
	}
	if (funcno == 0)
	{
		int count = getBoxCount();
		if(count < 0)
		{
			(GetDlgItem(IDC_EDIT1))->SetWindowText("can not find box");
			return;
		}
		sprintf(buff,"find box count:%d	",count);

	}
	if (itype == 1)
	{
		char* pFind = strrchr(szBuffer,'.');
		if (pFind)
			*pFind = '\0';
		strcat(szBuffer,".sql");

		if(!WriteMySqlByScript((LPSTR)(LPCTSTR)lpszSet,"Excute_Script",szBuffer,szerr))
		{
			strcat(buff,szerr);
			(GetDlgItem(IDC_EDIT1))->SetWindowText(buff);
			return;
		}
		else{
			strcat(buff,"write database success\n");
			(GetDlgItem(IDC_EDIT1))->SetWindowText(buff);
		}
	}
	else{
		strcat(buff,"parse ok\n");
		(GetDlgItem(IDC_EDIT1))->SetWindowText(buff);
	}
}

void CDxfReaderDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	static TCHAR strDirName[MAX_PATH];
	char buff[1024] = {0};
	char szErr[512] = {0};
	char cfilenames[1024][512];	
	int funcno = 0;

	(GetDlgItem(IDC_EDIT1))->SetWindowText(buff);

	vector<string>().swap(filenames);

	BROWSEINFO bi;
	CString szString = TEXT("选择一个源文件子文件夹");
	bi.hwndOwner = ::GetFocus();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = strDirName;
	bi.lpszTitle = szString;
	bi.ulFlags = BIF_BROWSEFORCOMPUTER | BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST pItemIDList = ::SHBrowseForFolder(&bi);
	if(pItemIDList == NULL)
	{
		return ;
	}
	::SHGetPathFromIDList(pItemIDList, strDirName);
	CString filepath = strDirName;


	CString sPath;
	GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer ();
	int nPos;
	nPos=sPath.ReverseFind ('\\');
	sPath=sPath.Left (nPos);
	CString lpszSet = sPath + "\\setting.ini";
	CString lpAppConvPath = sPath + "\\dd.exe";

	funcno  = ::GetPrivateProfileInt("General","funcno",0,(LPSTR)(LPCTSTR)lpszSet);

	FileSearch(filepath);

	if (filenames.size()>1024 || filenames.size() == 0)
	{
		(GetDlgItem(IDC_EDIT1))->SetWindowText("not dxf file or file count over 1024");
		return;
	}

	strcpy(buff,filepath);
	strcat(buff,"\\dxf_file_list.txt");
	FILE* log = openFile(buff,"wb+");

	for(int iloop = 0;iloop<filenames.size();iloop++){
		writeFile(log,filenames[iloop].c_str(),0);
		strcpy(cfilenames[iloop],filenames[iloop].c_str());
	}
	closeFile(log);

	memset(buff,0x00,1024);
	bool rt = dxfLoadDataFromFiles(cfilenames,filenames.size(),funcno,buff);
	//GetAllFiles(filepath,1);
	//FileSearch(filepath,1);
	if (rt)
	{
		(GetDlgItem(IDC_EDIT1))->SetWindowText("parse ok");
	}
	else{
		(GetDlgItem(IDC_EDIT1))->SetWindowText(buff);
		return;
	}

	strcpy(buff,cfilenames[0]);
	char* pFind = strrchr(buff,'.');
	if (pFind)
		*pFind = '\0';
	strcat(buff,".sql");

	if(!WriteMySqlByScript((LPSTR)(LPCTSTR)lpszSet,"Excute_Script",buff,szErr))
	{
		(GetDlgItem(IDC_EDIT1))->SetWindowText(szErr);
		return;
	}
	else{
		strcat(buff,"write database success\n");
		(GetDlgItem(IDC_EDIT1))->SetWindowText(buff);
		return;
	}
}

void CDxfReaderDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szBuffer[MAX_PATH] = {0}; 
	char szerr[512] = {0};
	OPENFILENAME ofn= {0};
	char buff[1024];  
	char sqlfile_name[MAX_PATH] = {0}; 
	FILE* fdb = NULL;

	(GetDlgItem(IDC_EDIT1))->SetWindowText("");

	ofn.lStructSize = sizeof(ofn); 
	ofn.hwndOwner = m_hWnd; 
	ofn.lpstrFilter = _T("txt文件(*.txt)\0*.txt\0");//要选择的文件后缀 
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区 
	ofn.nMaxFile = sizeof(szBuffer)/sizeof(*szBuffer); 
	ofn.nFilterIndex = 0; 
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER ;//标志如果是多选要加上OFN_ALLOWMULTISELECT
	BOOL bSel = GetOpenFileName(&ofn); 

	CString sPath;
	GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer ();
	int nPos;
	nPos=sPath.ReverseFind ('\\');
	sPath=sPath.Left (nPos);
	CString lpszSet = sPath + "\\setting.ini";

	strcpy(sqlfile_name,szBuffer);
	strcat(sqlfile_name,".sql");

	fdb = openFile(sqlfile_name,"wb+");
	addFileText(fdb);

	writeFile(fdb,"\n",0);

	if(txt2sql_wdb(fdb,szBuffer,szerr)!=0){
		(GetDlgItem(IDC_EDIT1))->SetWindowText(szerr);
		if (fdb)
			closeFile(fdb);
		return;
	}
	if (fdb)
		closeFile(fdb);

	strcpy(buff,"transform ok\n");
	(GetDlgItem(IDC_EDIT1))->SetWindowText(buff);
	//memset(szerr,0x00,512);
	//if(!WriteMySqlByScript((LPSTR)(LPCTSTR)lpszSet,"Excute_Script",sqlfile_name,szerr))
	//{
	//	//strcat(buff,szerr);
	//	(GetDlgItem(IDC_EDIT1))->SetWindowText(szerr);
	//	return;
	//}
	//else{
	//	strcpy(buff,"write database success\n");
	//	(GetDlgItem(IDC_EDIT1))->SetWindowText(buff);
	//}
}
