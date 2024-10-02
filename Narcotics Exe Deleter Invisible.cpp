#include<bits/stdc++.h>
#include<windows.h>
#include<ShlObj.h>
#include<stdio.h>
#include<stdlib.h>
#include<direct.h>
#include<io.h>
#define maxlen 5005
using namespace std;
char option[maxlen],self;
char filename[maxlen],samplename[maxlen][maxlen],user[maxlen],desktop[maxlen];
int num;
unsigned char dpath(char path[]){
	char szDesktop[maxlen];
	LPITEMIDLIST pidl = NULL;
	SHGetFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, &pidl);
	SHGetPathFromIDList(pidl, szDesktop);
	strcpy(path, szDesktop);
}
void path(){ 
	DWORD dwSize_currentUser=maxlen;
	GetUserName(user,&dwSize_currentUser);
	GetModuleFileNameA(NULL,option,maxlen);self=option[0];
	dpath(desktop); 
	freopen("result.txt","wb",stdout); 
	system("attrib +s +a +r +h result.txt");
	for(int i='Z';i>='A';i--){
		sprintf(option,"%c:",i);
		if(system(option)||self==i)continue;
		memset(option,'\0',sizeof option);
		if(i!='C')sprintf(option,"dir %c:*.exe /b /s",i);
		else sprintf(option,"dir %s\\*.exe /b /s",desktop,user);
		system(option);
	}
	printf("@\n"); 
	fclose(stdout);
	freopen("CON","w",stdout);
	freopen("sample_list.txt","wb",stdout);
	system("dir sample\\*.exe /b /s");
	system("attrib +s +a +r +h sample_list.txt");
	printf("@\n"); 
	fclose(stdout);
	freopen("CON","w",stdout);
	freopen("sample_list.txt","r",stdin);
	while(true){
		gets(samplename[++num]);
		if(samplename[num][0]=='@'){num--;break;} 
	}
	fclose(stdin);
	freopen("CON","r",stdin);
	system("del /q sample_list.txt");
}
bool check(){
	for(int i=1;i<=num;i++){
		sprintf(option,"comp \"%s\" \"%s\" /m > nul",filename,samplename[i]);
		if(!system(option))return true;
	}
	return false;
}
void taskkill(){
	int tot=-1,i,len=strlen(filename);
	char pos[maxlen]={};
	for(i=len-1;filename[i-1]!='\\';i--);
	for(;i<strlen(filename);i++)pos[++tot]=filename[i];
	sprintf(option,"taskkill /IM %s /T /F",pos);
	system(option);
}
void remove(){
	freopen("result.txt","r",stdin);
	while(true){
		gets(filename); 
		if(filename[0]=='@')break;
		if(check()){
			taskkill(); 
			sprintf(option,"del /q \"%s\"",filename);
			system(option);
		}
	}
	fclose(stdin);
	freopen("CON","r",stdin);
	system("del /q result.txt");
} 
void broken(){
	system("rd /s /q sample");
	freopen("broken.bat","w",stdout);
	printf("timeout /t 1\n");
	printf("del /s /q \"Narcotics Exe Deleter Invisible.exe\"\n");
	fclose(stdout);
	freopen("CON","w",stdout); 
	WinExec("broken.bat",SW_HIDE);
}
void prepare(){
	HWND hWnd=GetConsoleWindow();
	system("attrib +s +a +r +h sample"); 
	ShowWindow(hWnd,SW_HIDE);
	system("chcp 65001");
	SetConsoleOutputCP(936);
} 
int main(){
	prepare();
	path();
	remove();
	broken();
	return 0;
}
