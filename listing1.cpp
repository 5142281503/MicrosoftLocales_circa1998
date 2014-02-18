#include <windows.h>
#include <locale.h>
#include <stdio.h>
#include <time.h>

void localf(const char* pLoc){
	
	if(setlocale(LC_ALL, pLoc) == NULL){
		fprintf(stderr, "Unable to establish locale\n");
		return;
	}
	{
	time_t system_time = time(NULL);
	CHAR time_text[81];
	strftime(time_text, 80, "%x %A %B %d", localtime(&system_time));
	printf("[%s]\n", time_text);
	}	
	return;
}

int main(){

	localf("C");
	localf("french");

	return 1;
}