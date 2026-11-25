#include "widget.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QScrollBar>
#include <ctime>//新增
#include <QSizePolicy>
#include <QFont>
#include <QPointF>
#include <QLineF>
#include <QVector>
#include <QString>
#include"mapscene.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , initialScale(1.0)
{
    this->resize(1200, 900);

    // 创建布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // 创建场景和自定义视图
    scene = new MapScene(this);
    view = new CustomGraphicsView(this); // 使用自定义视图
    view->setScene(scene);

    layout->addWidget(view);

    // 加载地图数据
    scene->loadGeoJson(":/1.geojson");
    scene->loadGeoJson(":/2.geojson");

    // 创建菜单按钮
    menuButton = new MenuButton(this);
    menuButton->move(10, 10);
    menuButton->raise();

    // 创建弹出菜单
    popupMenu = new PopupMenu(this);

    // 连接信号
    connect(menuButton, &MenuButton::clicked, this, &Widget::onMenuButtonClicked);
    connect(popupMenu, &PopupMenu::button1Clicked, this, &Widget::onMenuButton1Clicked);
    connect(popupMenu, &PopupMenu::button2Clicked, this, &Widget::onMenuButton2Clicked);
    connect(popupMenu, &PopupMenu::button3Clicked, this, &Widget::onMenuButton3Clicked);
    connect(popupMenu, &PopupMenu::button4Clicked, this, &Widget::onMenuButton4Clicked);
    connect(popupMenu, &PopupMenu::button5Clicked, this, &Widget::onMenuButton5Clicked);
    connect(popupMenu, &PopupMenu::button6Clicked, this, &Widget::onMenuButton6Clicked);
    connect(popupMenu, &PopupMenu::button7Clicked, this, &Widget::onMenuButton7Clicked);
    connect(popupMenu, &PopupMenu::button8Clicked, this, &Widget::onMenuButton8Clicked);

    //新增: 创建时间显示的 UI
    uiOverlayTopRight = new QWidget(this);  // 创建左上角 UI 界面
    uiOverlayTopRight->setStyleSheet("background-color: rgba(255, 255, 255, 250);");  // 设置背景颜色和透明度
    timeLabel = new QLabel("时间: 00:00", uiOverlayTopRight);  // 创建时间标签
    timeLabel->setFixedSize(200, 40); // 设置固定大小，调整宽度和高度以适应文本
    QVBoxLayout* rightLayout = new QVBoxLayout(uiOverlayTopRight);
    rightLayout->addWidget(timeLabel);  // 将时间标签添加到布局中
    uiOverlayTopRight->setLayout(rightLayout);  // 设置布局
 //   layout->addWidget(uiOverlayTopRight); //添加到布局(用于测试)


    //新增 位置判定

    struct LineSegment {
        QVector<QPointF> points; // 线段的坐标点
        QString linename; // 线段的名称
    };
//    struct Building134, 22.6668378}, {114.2023999, 22.6672893}},
 //       "水官高速"
 /*   },
    {
        {{114.1903427, 22.6750957}, {114.1904823, 22.6752435}, {114.1907022, 22.6753661},
         {114.190853, 22.675536}, {114.1909563, 22.6756182}, {114.1913416, 22.6756133},
         {114.1916859, 22.6756416}, {114.1927115, 22.6760226}, {114.1939416, 22.6764974},
         {114.1942225, 22.6766276}, {114.1943834, 22.6767425}, {114.1945207, 22.676895},
         {114.1946619, 22.6770972}, {114.1948019, 22.6775228}, {114.1948716, 22.6776365},
         {114.1949711, 22.6777345}, {114.1959627, 22.6785441}, {114.196072, 22.6786607},
         {114.1960975, 22.6787382}, {114.1960697, 22.6788353}, {114.1959731, 22.6789125},
         {114.1956061, 22.6790969}, {114.1952777, 22.6792904}, {114.1950004, 22.6795154},
         {114.1943863, 22.6799809}, {114.1931536, 22.6808336}, {114.1925129, 22.6812757},
         {114.1918295, 22.6816706}, {114.1916617, 22.6817909}, {114.1916189, 22.6818485},
         {114.1916251, 22.681923}, {114.1916678, 22.6821216}},
        "服务路"
    },
    {
        {{114.1973721, 22.6815987}, {114.1963329, 22.6823524}, {114.1962356, 22.682423},
         {114.195615, 22.6828731}, {114.1943604, 22.6837935}},
        "未分类道路"
    },
    {
        {{114.1962356, 22.682423}, {114.1993476, 22.6842711}, {114.2003804, 22.6848977},
         {114.2023749, 22.6860666}, {114.2024129, 22.6860817}, {114.2024746, 22.6861062},
         {114.2025367, 22.6861309}, {114.2025769, 22.6861463}, {114.202714, 22.6861911},
         {114.202837, 22.6862236}, {114.2029666, 22.6862507}, {114.2031166, 22.6862716},
         {114.2032441, 22.6862818}, {114.2033675, 22.6862865}, {114.2051584, 22.686305},
         {114.2053814, 22.6863073}, {114.2054648, 22.6863081}, {114.2056653, 22.6863102},
         {114.2057818, 22.6862468}},
        "国际大学园路"
    },
    {
        {{114.2005216, 22.6918666}, {114.2021075, 22.6888987}},
        "大岭鼓隧道"
    },
    {
        {{114.2017068, 22.6885114}, {114.2002599, 22.6917012}},
        "大岭鼓隧道"
    },
    {
        {{114.2025425, 22.6867352}, {114.2017068, 22.6885114}},
        "盐龙大道"
    },
    {
        {{114.1979936, 22.6988353}, {114.1982855, 22.6972698}, {114.1983319, 22.6970606},
         {114.1983738, 22.6968986}, {114.1984429, 22.696638}, {114.1986901, 22.6957455},
         {114.1988066, 22.6954024}, {114.1989353, 22.6950496}, {114.1990907, 22.6946578},
         {114.1992591, 22.6942911}, {114.1994304, 22.6939298}, {114.1996184, 22.6935584},
         {114.2005216, 22.6918666}},
        "盐龙大道"
    },
    {
        {{114.2037585, 22.6858464}, {114.2039249, 22.6856492}, {114.2040693, 22.6854516},
         {114.2042012, 22.685264}, {114.2047011, 22.684565}, {114.2047742, 22.6844436},
         {114.2048504, 22.6842877}, {114.2049205, 22.6841079}, {114.2049674, 22.683945},
         {114.2050016, 22.6837393}, {114.2050221, 22.6835504}, {114.2050325, 22.6833452},
         {114.2050374, 22.6829991}},
        "主链接"
    },
    {
        {{114.2050374, 22.6829991}, {114.2050392, 22.6825097}, {114.205014, 22.6823683},
         {114.2049469, 22.6821818}, {114.2047422, 22.6817498}, {114.2046447, 22.6815526},
         {114.2045734, 22.6813706}, {114.2044981, 22.6810801}},
        "主链接"
    },
    {
        {{114.2058834, 22.686361}, {114.2057818, 22.6862468}, {114.2057005, 22.6861553},
         {114.2055666, 22.6859964}, {114.2054505, 22.6858458}, {114.2053293, 22.6856801},
         {114.205231, 22.6854968}},
        "龙翔大道"
    }
    };
QVector<Building> buildings = {
    {
        {{114.206332, 22.6889479}, {114.2065064, 22.6891167}, {114.2066636, 22.6889785},
         {114.2068421, 22.6891513}, {114.2066842, 22.6892897}, {114.2068919, 22.6894914},
         {114.207323, 22.6891134}, {114.2071153, 22.6889118}, {114.2067613, 22.6885703}},
        "知新楼"
    },
    {
        {{114.2052119, 22.6900388}, {114.2055927, 22.6903894}, {114.205795, 22.6902025},
         {114.2054142, 22.6898518}},
        "乐天楼"
    },
    {
        {{114.2057026, 22.6899022}, {114.2058611, 22.6900556}, {114.2062729, 22.6896934},
         {114.2061145, 22.68954}},
        "志仁楼"
    },
    {
        {{114.2059285, 22.6901407}, {114.206106, 22.6903083}, {114.2065249, 22.6899305},
         {114.2063475, 22.689763}},
        "诚道楼"
    },
    {
        {{114.206917, 22.6886446}, {114.2069241, 22.6886709}, {114.2069818, 22.6887235},
         {114.2069981, 22.6886998}, {114.2073151, 22.6890107}, {114.2073571, 22.6889719},
         {114.2074039, 22.6890188}, {114.2076031, 22.6888472}, {114.2074598, 22.6886615},
         {114.2073348, 22.6885499}, {114.2071996, 22.6884515}, {114.2071312, 22.6884736},
         {114.2070673, 22.6884184}, {114.206947, 22.688537}, {114.2069892, 22.6885735},
         {114.206917, 22.6886446}},
        "道远楼"
    },
    {
        {{114.2038837, 22.6889499}, {114.2046947, 22.6892317}, {114.2049745, 22.6885462},
         {114.2041635, 22.6882644}},
        "礼文楼"
    },
    {
        {{114.2078932, 22.6894517}, {114.2079241, 22.6894838}, {114.2079147, 22.6895075},
         {114.207999, 22.6895953}, {114.2080248, 22.6895742}, {114.2080666, 22.6896178},
         {114.2081831, 22.6895226}, {114.2080175, 22.6893501}},
        "未命名建筑"
    },
    {
        {{114.2066842, 22.6892897}, {114.2068919, 22.6894914}, {114.207323, 22.6891134},
         {114.2071153, 22.6889118}, {114.2068421, 22.6891513}},
        "图书馆"
    },
    {
        {{114.2052987, 22.6881015}, {114.2056287, 22.6884492}, {114.2063621, 22.6878396},
         {114.205944, 22.6873983}, {114.2056099, 22.6876894}},
        "室内体育馆"
    },
    {
        {{114.2058252, 22.6909272}, {114.2058758, 22.69098}, {114.2059232, 22.6910281},
         {114.2061694, 22.690822}, {114.2060717, 22.6907226}, {114.2058683, 22.6908929}},
        "逸夫书院D栋"
    },
    {
        {{114.2076741, 22.6903947}, {114.2078974, 22.6906202}, {114.2084849, 22.690125},
         {114.2082615, 22.6898994}},
        "涂辉龙楼"
    },
    {
        {{114.2080707, 22.6906372}, {114.2081989, 22.6907656}, {114.2085958, 22.6904283},
         {114.2089353, 22.6907683}, {114.2090686, 22.6906549}, {114.2086009, 22.6901865}},
        "实验楼A"
    },
    {
        {{114.2095044, 22.6919742}, {114.2096096, 22.6920933}, {114.2095092, 22.6921683},
         {114.2101596, 22.6920448}, {114.2097053, 22.6924154}, {114.209844, 22.6925566},
         {114.2104179, 22.6920713}, {114.2101787, 22.6918286}},
        "教学楼B"
    },
    {
        {{114.2090434, 22.6915152}, {114.2091473, 22.6916251}, {114.2090549, 22.6917095},
         {114.2096861, 22.6915683}, {114.2092175, 22.6919654}, {114.2094662, 22.6922169},
         {114.2096096, 22.6920933}, {114.2095044, 22.6919742}, {114.209954, 22.6915992}},
        "教学楼C"
    },
    {
        {{114.2083291, 22.6910295}, {114.2085623, 22.6912727}, {114.2092318, 22.691105},
         {114.2087543, 22.6914892}, {114.208997, 22.691746}, {114.2091473, 22.6916251},
         {114.2090434, 22.6915152}, {114.2095145, 22.6911362}, {114.2092466, 22.6908528}},
        "教学楼D"
    },
    {
        {{114.2076397, 22.6922281}, {114.2076745, 22.6922625}, {114.2076483, 22.692285},
         {114.2077946, 22.6924299}, {114.2078448, 22.6924}, {114.2078934, 22.6924579},
         {114.2080961, 22.6924704}, {114.2080777, 22.6926727}, {114.2083369, 22.6926935},
         {114.2083347, 22.6926424}, {114.2085492, 22.6926794}, {114.2085481, 22.6925515},
         {114.2083734, 22.6924572}, {114.2085006, 22.6923629}, {114.2085091, 22.6921027},
         {114.2082758, 22.6920805}, {114.2080603, 22.6918669}},
        "香港中文大学（深圳）大学图书馆"
    },
    {
        {{114.2100826, 22.6928056}, {114.210248, 22.6929725}, {114.2102165, 22.6929991},
         {114.2104168, 22.6932012}, {114.2106288, 22.6930223}, {114.2110286, 22.6926849},
         {114.2106882, 22.6923416}, {114.210558, 22.6924514}, {114.2105326, 22.6924258}},
        "教学楼A"
    },
    {
        {{114.1982593, 22.6932122}, {114.198398, 22.6932695}, {114.198484, 22.6930975},
         {114.198661, 22.693093}, {114.1986705, 22.6932784}, {114.1988379, 22.6933445},
         {114.1988953, 22.6932254}, {114.1988092, 22.6931901}, {114.1987949, 22.6929474},
         {114.1983836, 22.6929739}, {114.1982593, 22.6932122}},
        "学勤书院A座"
    },
    {
        {{114.1985518, 22.6936026}, {114.1988465, 22.6937327}, {114.1990574, 22.693326},
         {114.1989287, 22.6932692}, {114.1987785, 22.6935588}, {114.1986125, 22.6934856}},
        "学勤书院B座"
    },
    {
        {{114.1980077, 22.6937159}, {114.1984206, 22.6939007}, {114.1984729, 22.6937756},
         {114.1981987, 22.6936534}, {114.1983528, 22.6933492}, {114.1982203, 22.6932896}},
        "学勤书院C座"
    },
    {
        {{114.1975961, 22.6925543}, {114.1980985, 22.6927824}, {114.1981623, 22.6926628},
         {114.1976599, 22.6924347}},
        "思廷书院A座"
    },
    {
        {{114.1976599, 22.6931536}, {114.1976706, 22.693299}, {114.197857, 22.6932761},
         {114.1980388, 22.6929293}, {114.197897, 22.6928639}, {114.1977492, 22.6931539}},
        "思廷书院B座"
    },
    {
        {{114.1969965, 22.6929941}, {114.1972038, 22.6930856}, {114.1972216, 22.6933272},
         {114.1976706, 22.693299}, {114.1976599, 22.6931536}, {114.1976536, 22.6930572},
         {114.1970925, 22.6928093}},
        "思廷书院C座"
    },
    {
        {{114.1965475, 22.6932873}, {114.1969578, 22.6934677}, {114.197054, 22.6932814},
         {114.1970147, 22.6932641}, {114.1970467, 22.6932022}, {114.1966757, 22.6930391}},
        "服务中心"
    },
    {
        {{114.199026, 22.6924762}, {114.1990916, 22.6924997}, {114.1991433, 22.6924009},
         {114.1993427, 22.6924792}, {114.1992981, 22.6925743}, {114.1993767, 22.6926026},
         {114.1995025, 22.6923492}, {114.1991573, 22.6922113}},
        "祥波书院A座"
    },
    {
        {{114.2069984, 22.691554}, {114.2073239, 22.6918766}, {114.2077454, 22.6914924},
         {114.2076515, 22.6914012}, {114.2075985, 22.6913495}, {114.2074636, 22.6911486}},
        "学生中心"
    },
    {
        {{114.2089084, 22.6928704}, {114.2095511, 22.6935356}, {114.2096885, 22.6934226},
         {114.2100141, 22.6937596}, {114.2101916, 22.6936136}, {114.2096989, 22.693104},
         {114.2095296, 22.6932433}, {114.2093423, 22.6930496}, {114.209513, 22.6929092},
         {114.2093315, 22.6927215}, {114.2091609, 22.6928618}, {114.2090537, 22.6927509}},
        "行政楼"
    },
    {
        {{114.2094353, 22.6941944}, {114.2097524, 22.6944944}, {114.2101305, 22.6947556},
         {114.2105123, 22.6949255}, {114.2108056, 22.6950689}, {114.2112672, 22.6953528},
         {114.2115588, 22.6955363}, {114.2119908, 22.6957629}, {114.2123874, 22.6963043}},
        "逸夫国际会议中心"
    }
};
   // 判定红色点是否与任意线段重合

    for (const auto& line : lines) {
        for (int i = 0; i < line.points.size() - 1; ++i) {
            QLineF segment(line.points[i], line.points[i + 1]);
            bool pointOnLine = false;
            // 获取红点坐标
            MapScene* mapScene = new MapScene(this);
            ClickableEllipseItem* controlPoint = mapScene->getControlPoint();
            if (controlPoint) {
                QPointF controlPointPos = controlPoint->pos();
                redDotX = controlPointPos.x();
                redDotY = controlPointPos.y();
            }
            if (segment.contains(*controlPointPos)) {
                location = line.name; // 更新 location 为该线段的名称
                pointOnLine = true;
                break; // 找到重合，退出循环
            }
        }
        if (pointOnLine) break; // 如果已找到重合，退出外层循环
    }
*/
    //新增: 创建角色状态显示的 UI
    uiOverlayTopRight2 = new QWidget(this);  // 创建右上角 UI 界面
    uiOverlayTopRight2->setStyleSheet("background-color: rgba(255, 255, 255, 250);");  // 设置背景颜色和透明度
    statusLabel = new QLabel("角色状态: ", uiOverlayTopRight2);  // 创建角色状态标签
    QVBoxLayout* rightLayout2 = new QVBoxLayout(uiOverlayTopRight2);
    rightLayout2->addWidget(statusLabel);  // 将状态标签添加到布局中
    uiOverlayTopRight2->setLayout(rightLayout2);  // 设置布局
 //   layout->addWidget(uiOverlayTopRight2); //添加到布局底部（用于测试）
    /*
     //创建NPC状态显示的UI
    uiNPC = new QWidget(this);  // 创建NPC界面
    uiNPC->setStyleSheet("background-color: rgba(255, 255, 255, 250);");  // 设置背景颜色和透明度
    NPCLabel = new QLabel("角色状态: ", uiNPC);  // 创建角色状态标签
    QVBoxLayout* npcLayout = new QVBoxLayout(uiNPC);
    npcLayout->addWidget(NPCLabel);  // 将状态标签添加到布局中
    uiNPC->setLayout(npcLayout);  // 设置布局
    layout->addWidget(uiNPC); //添加到布局底部（用于测试）
    */
    uiTask = new QWidget(this);  // 创建NPC界面
    uiTask->setStyleSheet("background-color: rgba(255, 255, 255, 250);");  // 设置背景颜色和透明度
    TaskLabel = new QLabel("任务栏: ", uiTask);  // 创建角色状态标签
    QVBoxLayout* taskLayout = new QVBoxLayout(uiTask);
    taskLayout->addWidget(TaskLabel);  // 将状态标签添加到布局中
    uiTask->setLayout(taskLayout);  // 设置布局
    layout->addWidget(uiTask); //添加到布局底部（用于测试）
    //新增: 定时器更新游戏时间
    struct timeclock
    {
        int year;
        int month;
        int date;
        int hour;
        int min;
    };
    time_t now = time(0);

    static timeclock Game_Time;

    tm *ltm = localtime(&now);
    Game_Time.year = ltm->tm_year + 1900;
    Game_Time.month = ltm->tm_mon + 1;
    Game_Time.date = ltm->tm_mday;
    Game_Time.hour = ltm->tm_hour;
    Game_Time.min = ltm->tm_min;//设置游戏时间变量并将其初始化为游戏开始时间


//调整标签以及ui大小
        // 设置字体和大小
        QFont font;
        font.setPointSize(12); // 设置字体大小
        statusLabel->setFont(font);
        timeLabel->setFont(font);
 //     NPCLabel->setFont(font);
        TaskLabel->setFont(font);
        timeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        statusLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//      NPCLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        TaskLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        // 设置标签的固定大小
        uiOverlayTopRight->setFixedSize(600, 50);
        uiOverlayTopRight2->setFixedSize(600, 50);
        timeLabel->setFixedSize(600, 50);
        statusLabel->setFixedSize(600, 50);
        timeLabel->setWordWrap(true);
        statusLabel->setWordWrap(true);

QTimer* timer = new QTimer(this);
connect(timer, &QTimer::timeout, [this]() {

    // 获取红点坐标
    MapScene* mapScene = new MapScene(this);
    ClickableEllipseItem* controlPoint = mapScene->getControlPoint();

    if (controlPoint) {

        QPointF controlPointPos = controlPoint->pos();
        redDotX = controlPointPos.x();
        redDotY = controlPointPos.y();
    }
    // 每秒钟增加8分钟
    Game_Time.min += 8;
    //饱食度每秒下降
    hungerValue -= 0.3;
    energyValue -= 0.1;
    //处理健康值
    if ((hungerValue < 50 || energyValue < 30)&&(healthValue >=0)){
        if(hungerValue >= 30 || energyValue >= 10){
            healthValue -=0.5;
        }
        else if((hungerValue < 30 || energyValue < 10)&&(hungerValue >0)&&( energyValue > 10)){
            healthValue -=2;
        }
        else if(hungerValue ==0 || energyValue ==0){
            healthValue = 0;
        }
    }
    if (healthValue<=0){
        healthValue = 0;
        exit(0);
    }
    // 处理分钟溢出
    if (Game_Time.min >= 60) {
        Game_Time.hour += Game_Time.min / 60;
        Game_Time.min %= 60;

        // 处理小时溢出
        if (Game_Time.hour >= 24) {
            Game_Time.date += Game_Time.hour / 24;
            Game_Time.hour %= 24;

            // 处理日期溢出（简化处理，不考虑不同月份的天数）
            if (Game_Time.date > 30) { // 假设每月30天
                Game_Time.date = 1;
                Game_Time.month++;
                if (Game_Time.month > 12) {
                    Game_Time.month = 1;
                    Game_Time.year++;
                }
            }
        }
    }
// 更新状态标签
    statusLabel->setText(QString("角色状态: 健康值:%1, 饱食度:%2,精力值:%3,所在位置: %4%5%6")
        .arg(healthValue)
        .arg(hungerValue)
        .arg(energyValue)
        .arg(location)
        .arg(redDotX)
        .arg(redDotY));

 // 更新时间标签
    timeLabel->setText(QString("时间: %1-%2-%3 %4:%5")
        .arg(Game_Time.year)
        .arg(Game_Time.month, 2, 10, QChar('0'))
        .arg(Game_Time.date, 2, 10, QChar('0'))
        .arg(Game_Time.hour, 2, 10, QChar('0'))
        .arg(Game_Time.min, 2, 10, QChar('0')));   // 格式化为两位数

});
timer->start(1000);  // 每秒更新时间
//调整ui位置
    uiOverlayTopRight->move(600, 60); // 右上角
    uiOverlayTopRight2->move(600, 0);

}

Widget::~Widget()
{
}

void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    fitViewToScene();
}

void Widget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    fitViewToScene();
}

void Widget::onMenuButtonClicked()
{
    QPoint pos = menuButton->mapToGlobal(QPoint(0, menuButton->height()));
    popupMenu->move(pos);
    popupMenu->show();
}

void Widget::onMenuButton1Clicked()
{

}

void Widget::onMenuButton2Clicked()
{

}

void Widget::onMenuButton3Clicked()
{

}

void Widget::onMenuButton4Clicked()
{

}

void Widget::onMenuButton5Clicked()
{

}

void Widget::onMenuButton6Clicked()
{

}

void Widget::onMenuButton7Clicked()
{

}

void Widget::onMenuButton8Clicked()
{

}

void Widget::fitViewToScene()
{
    if (view && scene) {
        // 获取场景矩形
        QRectF sceneRect = scene->sceneRect();
        if (sceneRect.isEmpty()) {
            return;
        }

        // 重置变换
        view->resetTransform();

        // 适应视图，并确保完全填充
        view->fitInView(sceneRect, Qt::KeepAspectRatio);

        // 您之前可能进行了额外的缩放，例如：
        // view->scale(1.5, 1.5);

        // 获取视图的当前变换矩阵
        QTransform transform = view->transform();

        // 计算缩放因子（假设缩放在 x 和 y 方向相同）
        double scaleFactor = transform.m11();

        // 设置 CustomGraphicsView 的 currentScaleFactor，并更新标注可见性
        view->setCurrentScaleFactor(scaleFactor);

        // 更新视图
        view->viewport()->update();
    }
}
