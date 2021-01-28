
__G__TRACKBACK__ = function(msg)
    local msg = debug.traceback(msg, 3)
    print(msg)
    return msg
end

require "res.lib.header"
require "res.config"

-- 控件初始化
function initCtrl()
    print("initCtrl in")
    local listKj = thMFCManager:getListCtrl("listKj")
	listKj:InsertColumn(0, "test1", 120)
	listKj:InsertColumn(1, "test2", 80)
end
 
-- main
local function main()
    print("main in")
    initCtrl()
    thHttpManage:websiteGetEx("_t","https://www.baidu.com",false,function(value)
        local code = value:asInt("code")
        local data = value:asString("data")
        print("baidu->",code,data)
    end)
end
 
local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    print(msg)
end