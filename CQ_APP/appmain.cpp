#include <CQSDK.h>
#include "appinfo.h"

#include "util/common.h"
#include "app.h"

MUST_AppInfo_RETURN(APP_ID)

EVE_Startup_EX(Startup)
//name:酷Q启动
//priority:30000
/*
以上2行为注释,用于json自动生成
name为事件名称,一般标识用途,不需要表明事件类型
priority为事件优先级(参见 cq.im/deveventpriority)
开头必须为<//name:>或<//priority:>,不得有空格等
如果没有name,默认为事件名
如果没有priority,默认为30000
不要包含{字符,否则视为结束
*/

{
	qff233::InitLogger();
	qff233::init();
	//本函数在执行周期最多只会执行一次
	//可以在这里加载全局资源
}






EVE_Enable_EX(Enable)
//name:禁用将无法加载数据
//priority:30000
{
	//应用有可能多次启用
	//可以在这里加载运行周期内才需要的资源
}






EVE_Disable_EX(Disable)
//name:禁用将不能保存数据
//priority:30000
{
	//应用有可能多次停止
	//停止后dll还会继续运行
	//但是收不到任何事件也不能调用任何API
}






EVE_Exit_EX(Exit)
//name:退出清理
//priority:30000
{
	//本函数执行以后,酷Q会短时间内结束运行
}
