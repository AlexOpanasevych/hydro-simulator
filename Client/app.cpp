#include "app.h"

App::App(QObject *parent) : QObject(parent)
{
    setMenuModel({QVariantMap({
                      {
                          "menuTitle", "Під'єднатися до сервера"
                      }
//                      {
//                          "callback", QVariant::fromValue(std::function<void()>([this](){

//                          }))
//                      }
                  }),
//                  QVariantMap({
//                      {
//                          "menuTitle", "Оновити ПО"
//                      }
////                      {
////                          "callback", QVariant::fromValue(std::function<void()>([this](){
////                              updateService()->loadAll();
////                          }))
////                      }
//                  }),
                  QVariantMap({
                                        {
                                            "menuTitle", "Змінити режим"
                                        }
                  //                      {
                  //                          "callback", QVariant::fromValue(std::function<void()>([this](){
                  //                              updateService()->loadAll();
                  //                          }))
                  //                      }
                                    })
                  }
                 );
    funcList.append([this](){
        this->client()->connect();
    });
//    funcList.append([this](){
//          updateService()->loadAll();
//    });
    funcList.append([this](){
        if(m_viewMode == INTERACTIVE) {
            setViewMode(HELP);
        } else {setViewMode(INTERACTIVE);}
    });

//    updateService()->loadAll();
}

App::~App() {
//    updateService()->deleteLater();
    client()->deleteLater();
}
