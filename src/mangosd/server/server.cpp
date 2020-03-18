#include "lib/cpp-httplib/httplib.h"
#include "Config/Config.h"
#include "Accounts/AccountMgr.h"
#include "server.h"
#include "Globals/ObjectAccessor.h"


class ServerRunnable : public MaNGOS::Runnable{
  public:
    void run(void)
    {
        using namespace httplib;
        Server svr;

        // AccountMgr* mgr = new AccountMgr();

        svr.Get("/kick", [](const Request& req, Response& res) {
            Player* player = ObjectAccessor::FindPlayerByName("Typescript");
            if(player){
              ObjectAccessor::KickPlayer(player->GetObjectGuid());
            }else{
              res.set_content("not exists", "text/plain");
            }
            
        });
        //头 颈 肩 衬衣 胸 腰 腿 脚 手腕 手 戒指1 戒指2 饰品1 饰品2 背部 主手 副手 远程 战袍 

        

        svr.Get("/createAccount", [](const Request& req, Response& res) {
          std::string username =  req.get_param_value("username");
          std::string password =  req.get_param_value("password");
          AccountOpResult result = sAccountMgr.CreateAccount(username, password);
          std::string t;
          switch (result)
          {
              case AOR_OK:
                  res.set_content("1", "text/plain");
                  break;
              case AOR_NAME_TOO_LONG:
                  res.set_content("2", "text/plain");
                  return false;
              case AOR_NAME_ALREADY_EXIST:
                  res.set_content("3", "text/plain");
                  return false;
              case AOR_DB_INTERNAL_ERROR:
                  res.set_content("4", "text/plain");
                  return false;
              default:
                  res.set_content("5", "text/plain");
                  return false;
          }

        });

        svr.Get(R"(/numbers/(\d+))", [&](const Request& req, Response& res) {
          auto numbers = req.matches[1];
          res.set_content(numbers, "text/plain");
        });

        svr.Get("/stop", [&](const Request& req, Response& res) {
          svr.stop();
        });

        svr.listen("localhost", 1234);
    }
};

void extension_server_start()
{
  new MaNGOS::Thread(new ServerRunnable);
};