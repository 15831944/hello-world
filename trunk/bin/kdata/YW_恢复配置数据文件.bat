set date=%date:~0,4%%date:~5,2%%date:~8,2%
if %time:~0,2% leq 9 (set hour=0%time:~1,1%) else (set hour=%time:~0,2%)
set min=%time:~3,2%%time:~6,2%
set DST=%date%%hour%%min%




::ע����д�����ļ�������  ����+ʱ��
#set Release8=YW_��̻������ļ�����(%DST%)
set Release8=YW_��̻������ļ�����

::�����ﴴ���ļ���
#mkdir %Release8%  

#mkdir %Release8%\ico
#mkdir %Release8%\kdata 
#mkdir %Release8%\pdata
#mkdir %Release8%\picture
#mkdir %Release8%\report




::ע����дҪ���ݵ��ļ�

::������Ϣ�ļ� ����Ŀ¼
copy  "%Release8%\Knife.ini" 						".\"	

::kdata�µ������ļ� �����ļ�
copy  	"%Release8%\kdata\Excel.ywe"				"kdata"
copy  	"%Release8%\kdata\knifetype.ywe"			"kdata"
copy   	"%Release8%\kdata\leftpanel.ywe"			"kdata"
copy   	"%Release8%\kdata\Op.ywe"					"kdata"
copy   	"%Release8%\kdata\printcode.ywe"			"kdata"
copy   	"%Release8%\kdata\PrintModelSelect.ywe"		"kdata"
copy 	"%Release8%\kdata\SlotTemplate.ywe"			"kdata"
copy   	"%Release8%\kdata\UISetting.ywe"			"kdata"
copy  	"%Release8%\kdata\VecSolt.ywe"				"kdata"
copy   	"%Release8%\kdata\VecSoltChange.ywe"		"kdata"


::picture�µ������ļ� 
copy  	"%Release8%\picture\printcode.ywe"			"picture" 


::report�µ������ļ�
copy  	"%Release8%\report\lingliaobiao.xml"		"report" 			


::Ҫ����ʲô�ļ���������ĸ�ʽ��д�Ϳ�����






