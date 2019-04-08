#include <stdio.h>
#include "cgic.h"
#include "ctemplate.h"

#define HTML "../html/calc.html"

int cgiMain()
{
	char numa[256] = {0};
	char numb[256] = {0};
	char result[256] = {0};
	int ret = 0;
	TMPL_varlist *list = NULL;
	

	if(cgiFormSubmitClicked("calc") == cgiFormSuccess){

		cgiFormString("numa", numa, sizeof(numa));// 增加判断返回 第一个参数必填
		cgiFormString("numb", numb, sizeof(numb));//增加判断返回 第二个参数必填
		ret = atoi(numa) + atoi(numb);//增加判断返回 填入必须为数字
		sprintf(result, "%d", ret);
		list = TMPL_add_var(list, "numa", numa, "numb", numb, "result", result, NULL);
		
		cgiHeaderContentType("text/html; charset=utf-8");
		fprintf(cgiOut, "numa :%s, numb:%s, result:%s\n", numa, numb, result);

		
		TMPL_write(HTML, NULL, NULL, list, cgiOut, cgiOut);
	}else{
		list = TMPL_add_var(list, "numa", "", "numb", "", "result", "", NULL);
		
		cgiHeaderContentType("text/html; charset=utf-8");
		TMPL_write(HTML, NULL, NULL, list, cgiOut, cgiOut);

	}

	return 0;
}

