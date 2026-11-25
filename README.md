# University Campus Tour Map — CUHKSZ Simulation Game System

香港中文大学（深圳）CSC3002 Group 36 课程项目的校园地图与角色状态原型，使用 **C++17、Qt Widgets 和 qmake** 开发。

程序将 GeoJSON 道路和建筑数据绘制为可缩放的二维校园地图，并展示玩家控制点、沿预设路线移动的 NPC、游戏时间和角色状态。

> **项目范围：** 本仓库保存的是当前归档中的 `mapView` 模块。小组报告介绍了更完整的校园模拟系统，其中的选课系统、2048、钢琴块、音乐播放器和留言板等模块源码未包含在本仓库中。

## 当前功能

| 内容 | 当前实现 |
| --- | --- |
| 校园地图 | 加载 GeoJSON 道路与建筑数据，通过 Qt Graphics View 绘制 |
| 地图浏览 | 鼠标滚轮缩放、按住鼠标左键拖动；文字随缩放级别显示或隐藏 |
| 玩家移动 | 使用 W、A、S、D 控制红色角色点，包含加速、限速和减速逻辑 |
| NPC 移动 | 蓝色点沿预设道路来回移动 |
| 时间与状态 | 显示游戏时间、健康值、饱食度和精力值；定时推进时间并更新属性 |
| 建筑点击 | 部分建筑可以响应点击，目前主要输出调试信息 |
| 功能菜单 | 已有弹出菜单界面；八个功能按钮的业务回调尚未实现 |

地图数据通过 Qt 资源系统编入程序，运行时无需另外指定地图文件路径。

## 目录结构

```text
.
├── README.md
├── .gitignore
├── docs/
│   ├── group36-report.docx     # 原小组报告
│   └── UPLOAD_GITHUB.md        # GitHub 上传步骤
└── mapView/
    ├── mapView.pro             # qmake 工程文件
    ├── main.cpp                # 程序入口
    ├── widget.cpp / .h         # 主界面、时间与角色状态
    ├── mapscene.cpp / .h       # 地图解析、绘制与角色移动
    ├── customgraphicsview.*    # 地图缩放和拖动
    ├── clickableellipseitem.*  # 可点击的角色/NPC 图元
    ├── clickablepathitem.*     # 可点击的建筑图元
    ├── menubutton.*            # 菜单入口
    ├── popupmenu.*             # 弹出菜单
    └── mapData/
        ├── 1.geojson           # 道路数据
        ├── 2.geojson           # 建筑数据
        └── resources.qrc       # Qt 资源清单
```

本地原有的 `mapView/build/`、Qt Creator 的 `*.pro.user*` 配置和根目录的 `mapView.zip` 由 `.gitignore` 排除，不会进入正常的首次提交。

## 开发环境

- C++17 编译器。
- Qt 6.5.3，包含 Core、Gui、Widgets 和 qmake。
- Qt Creator，或已配置好 Qt 的命令行环境。
- Windows 下可使用 **Qt 6.5.3 MinGW 64-bit + MinGW 11.2.0**，与原项目的构建配置对应。

当前模块不依赖报告中其他模块使用的 EasyX，也不需要数据库或在线地图服务。

## 构建与运行

### 使用 Qt Creator

1. 安装 Qt 6.5.3、MinGW 64-bit Kit 和 Qt Creator。
2. 在 Qt Creator 中打开 `mapView/mapView.pro`。
3. 选择匹配的 **Desktop Qt 6.5.3 MinGW 64-bit** Kit。
4. 选择独立的构建目录，例如仓库根目录下的 `build/`。
5. 点击构建并运行。

仓库不包含作者电脑的 `*.pro.user*` 文件；首次打开时由 Qt Creator 生成本机配置。

### Windows 命令行

先将**同一套 Kit** 的 Qt `bin` 和 MinGW `bin` 加入当前终端的 `PATH`，确保 `qmake` 与 `mingw32-make` 可用。随后在仓库根目录执行：

```powershell
New-Item -ItemType Directory -Path build -Force | Out-Null
Set-Location build
qmake ..\mapView\mapView.pro -spec win32-g++ "CONFIG+=release" "CONFIG-=debug"
mingw32-make -j4
.\release\mapView.exe
```

运行程序时也需要匹配的 Qt DLL 可被找到；使用 Qt Creator 或已设置 Qt `bin` 路径的终端运行即可。如果后续需要提供独立的 Windows 程序包，可使用同一 Kit 的 `windeployqt` 收集运行依赖，再作为 GitHub Release 附件发布。

**构建验证：** 整理后的源码已在 Windows 上使用 Qt 6.5.3 / MinGW 11.2.0 64-bit 完成独立的 Release 构建；头文件引用、Qt 资源、GeoJSON、报告完整性及忽略规则检查通过。尚未进行完整的界面交互测试，也未验证 Linux 或 macOS 构建。

## 操作方式

| 操作 | 输入 |
| --- | --- |
| 缩放地图 | 鼠标滚轮 |
| 拖动地图 | 按住鼠标左键并移动 |
| 移动红色角色点 | W / A / S / D；先点击地图使视图获得键盘焦点 |
| 查看建筑点击响应 | 点击部分建筑，在 Qt Creator 的应用输出中查看调试信息 |
| 打开功能菜单 | 点击左上角的 `≡` 按钮 |

## 已知限制

- 这是课程原型，功能菜单和角色/NPC 点击回调仍有占位实现，尚未接入报告中的其他模块。
- 角色状态栏的地点识别逻辑尚未完成，位置与坐标显示不能视为可靠的定位结果。
- 状态刷新逻辑中存在重复创建地图场景的情况，长时间运行时的对象管理仍需完善。
- 当前时间模型采用简化日历；健康值降至零时直接退出程序。
- 界面主要面向中文和桌面窗口，部分控件使用固定尺寸；其他分辨率和平台的显示效果尚未验证。

## 报告、署名与数据

完整小组项目的背景、设计与各子系统说明见 [Group 36 项目报告](docs/group36-report.docx)。报告覆盖的内容多于本仓库实际保存的源码范围。

原项目归属 CSC3002 Group 36；个人分工与贡献可根据小组实际情况补充。本仓库尚未声明开源许可证。

两份 GeoJSON 数据保留原文件，其中带有 `osm_id` 等字段；原始归档未附完整的数据下载地址和许可说明，数据来源、许可与所需署名仍待补齐。

项目仓库：[Fa1znan/University-Campus-Tour-Map-CUHKSZ-Simulation-Game-System](https://github.com/Fa1znan/University-Campus-Tour-Map-CUHKSZ-Simulation-Game-System)。

上传与后续更新步骤见 [GitHub 上传指南](docs/UPLOAD_GITHUB.md)。
