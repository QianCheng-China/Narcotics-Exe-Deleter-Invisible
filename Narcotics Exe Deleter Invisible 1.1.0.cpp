#include<bits/stdc++.h>
#include<windows.h>
#include<ShlObj.h>
#include<stdio.h>
#include<stdlib.h>
#include<direct.h>
#include<io.h>
#define maxlen MAX_PATH 
using namespace std;
char option[maxlen*10],tmp[maxlen];
char FilePath[maxlen],SampPath[105][maxlen],DeskPath[maxlen],SelfPath[maxlen],WorkPath[maxlen],TempPath[maxlen];
char New_SFN[maxlen];
int SampNum=0;
void encrypt(char name[]){
	char code[15]={};
	int chos;
	for(int i=0;i<rand()%5+5;i++){
		chos=rand()%3;
		if(chos==0)code[i]='A'+rand()%26;
		else if(chos==1)code[i]='a'+rand()%26;
		else code[i]='0'+rand()%10;
	}
	strcpy(name,code);
}
void GetPath(){ 
	getcwd(WorkPath,maxlen);
	SHGetSpecialFolderPath(0,DeskPath,CSIDL_DESKTOPDIRECTORY,0);
	GetModuleFileName(NULL,SelfPath,maxlen);
	GetTempPath(MAX_PATH,TempPath);
	encrypt(New_SFN);
	sprintf(option,"rename sample %s",New_SFN);system(option); 
	freopen("sample_list.txt","w",stdout);
	sprintf(option,"dir %s\\*.exe /b /s",New_SFN);
	system(option); 
	printf("%c\n",'|'); 
	fclose(stdout);freopen("CON","w",stdout);
	freopen("sample_list.txt","r",stdin);
	while(true){
		gets(SampPath[++SampNum]);
		if(SampPath[SampNum][0]=='|'){SampNum--;break;}
		encrypt(tmp);
		sprintf(option,"rename \"%s\" \"%s\"",SampPath[SampNum],tmp);system(option);
		sprintf(option,"%s\\%s\\%s",WorkPath,New_SFN,tmp);strcpy(SampPath[SampNum],option);	
	}
	fclose(stdin);freopen("CON","r",stdin);
	sprintf(option,"echo attrib +a +r +s +h %s > opt.bat",New_SFN);system(option);
	system("echo exit >> opt.bat");
	system("start opt /MIN /B");
	sprintf(option,"md %s",TempPath);system(option);
	sprintf(option,"%s\\file_list.txt",TempPath);
	freopen(option,"w",stdout);
	for(int i='Z';i>='A';i--){
		sprintf(option,"%c:",i);
		if(system(option))continue;
		if(i=='C')sprintf(option,"dir %s\\*.exe /b /s",DeskPath);
		else sprintf(option,"dir %c:\\*.exe /b /s",i);
		system(option);
	}
	printf("%c\n",'|');
	fclose(stdout);freopen("CON","w",stdout);
	system("del /q sample_list.txt");
	system("del /q opt.bat");
}
bool Check(){
	for(int i=1;i<=SampNum;i++){
		sprintf(option,"comp \"%s\" \"%s\" /m > nul",FilePath,SampPath[i]);
		if(!system(option))return true;
	}
	return false;
}
void Taskkill(){
	int tot=-1,i,len=strlen(FilePath);
	char pos[maxlen]={};
	for(i=len-1;FilePath[i-1]!='\\';i--);
	for(;i<strlen(FilePath);i++)pos[++tot]=FilePath[i];
	sprintf(option,"taskkill /IM %s /T /F",pos);system(option);
}
void Remove(){
	sprintf(option,"%s\\file_list.txt",TempPath);
	freopen(option,"r",stdin);
	while(true){
		gets(FilePath); 
		if(FilePath[0]=='|')break;
		if(Check())Taskkill(),sprintf(option,"del /q \"%s\"",FilePath),system(option);
	}
	fclose(stdin);freopen("CON","r",stdin);
} 
void Broken(){
	sprintf(option,"attrib -a -r -s -h %s",New_SFN);system(option);
	sprintf(option,"rd /s /q %s",New_SFN);system(option);
	system("echo timeout /t 1 > broken.bat");
	sprintf(option,"echo del /s /q /f \"%s\" >> broken.bat",SelfPath);system(option);
	sprintf(option,"del /q %s\\file_list.txt",TempPath); 
	system(option);
	WinExec("broken.bat",SW_HIDE);
}
void Prepare(){
	srand(GetTickCount());
	HWND hWnd=GetConsoleWindow();
	ShowWindow(hWnd,SW_HIDE); 
	system("chcp 65001");
	SetConsoleOutputCP(936);
} 
int main(){
	Prepare();
	GetPath();
	Remove();
	Broken();
	return 0;
}
