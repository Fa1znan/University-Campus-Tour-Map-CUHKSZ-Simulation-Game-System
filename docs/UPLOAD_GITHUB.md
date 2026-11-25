# 上传与更新 GitHub 仓库

项目使用已经创建的仓库：

[Fa1znan/University-Campus-Tour-Map-CUHKSZ-Simulation-Game-System](https://github.com/Fa1znan/University-Campus-Tour-Map-CUHKSZ-Simulation-Game-System)

本地目录为 `D:\Courses files\CSC3002`。本地文件夹无需与远程仓库同名。

## 检查本地状态

在 PowerShell 中执行：

```powershell
Set-Location 'D:\Courses files\CSC3002'
git status
git remote -v
```

`origin` 应指向：

```text
https://github.com/Fa1znan/University-Campus-Tour-Map-CUHKSZ-Simulation-Game-System.git
```

本地若已存在 `.git` 且已连接此地址，无需再次初始化或重复添加 `origin`。

## 首次上传的命令

以下流程仅适用于尚未初始化 Git 的项目副本，且远程仓库仍为空：

```powershell
Set-Location 'D:\Courses files\CSC3002'
git init -b main
git add .
git status --short
git commit -m "Initial commit: campus map and character interaction"
git remote add origin https://github.com/Fa1znan/University-Campus-Tour-Map-CUHKSZ-Simulation-Game-System.git
git push -u origin main
```

待提交内容应包括 `README.md`、`.gitignore`、`docs/`、`mapView/` 源码及地图资源。`mapView/build/`、`*.pro.user*` 和根目录的 `mapView.zip` 已被 `.gitignore` 排除。

如果 Git 提示缺少提交者身份，使用自己的 GitHub 用户名和 GitHub Settings → Emails 页面提供的提交邮箱配置当前仓库：

```powershell
git config user.name "Fa1znan"
git config user.email "填写 GitHub 提供的 noreply 提交邮箱"
```

## 后续修改并上传

在仓库根目录执行：

```powershell
Set-Location 'D:\Courses files\CSC3002'
git add .
git status --short
git commit -m "Describe the changes"
git push
```

如果首次推送尚未成功，运行 `git push -u origin main` 完成首次推送并设置上游分支。出现登录窗口或浏览器授权提示时，按提示完成 GitHub 登录；HTTPS 需要令牌时使用 Personal Access Token。

## 后续可补充的展示内容

将真实运行截图放入 `docs/screenshots/` 并在 README 中引用。需要分享可直接运行的 Windows 程序时，使用匹配 Kit 的 `windeployqt` 收集 Qt 运行依赖，将程序包发布到 GitHub Releases。
