set date=%date:~0,4%%date:~5,2%%date:~8,2%
if %time:~0,2% leq 9 (set hour=0%time:~1,1%) else (set hour=%time:~0,2%)
set min=%time:~3,2%%time:~6,2%
set DST=%date%%hour%%min%




::ע����д�����ļ�������  ����+ʱ��
#set Release8=��̻������ļ�����(%DST%)
set Release8=��̻������ļ�����

::�����ﴴ���ļ���
mkdir %Release8%  

::mkdir %Release8%\ico
mkdir %Release8%\kdata 
::mkdir %Release8%\pdata
mkdir %Release8%\picture
mkdir %Release8%\report




::ע����дҪ���ݵ��ļ�

::������Ϣ�ļ� ����Ŀ¼
copy  "Knife.ini" 							"%Release8%" 	

::kdata�µ������ļ� �����ļ�
copy  "kdata\Excel.hge" 					"%Release8%\kdata"	
copy  "kdata\knifetype.hge" 				"%Release8%\kdata"	
copy  "kdata\leftpanel.hge" 				"%Release8%\kdata"	
copy  "kdata\Op.hge" 						"%Release8%\kdata"
copy  "kdata\printcode.hge" 				"%Release8%\kdata"	
copy  "kdata\PrintModelSelect.hge" 			"%Release8%\kdata"	
copy  "kdata\SlotTemplate.hge" 				"%Release8%\kdata"	
copy  "kdata\UISetting.hge" 				"%Release8%\kdata"	
copy  "kdata\VecSolt.hge" 					"%Release8%\kdata"	
copy  "kdata\VecSoltChange.hge" 			"%Release8%\kdata"	


::picture�µ������ļ� �����ļ�
copy  "picture\printcode.hge" 				"%Release8%\picture"	


::report�µ������ļ� �����ļ�
copy  "report\lingliaobiao.xml" 			"%Release8%\report"	


::Ҫ����ʲô�ļ���������ĸ�ʽ��д�Ϳ�����






