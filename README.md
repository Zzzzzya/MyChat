# MyChat
一个即时通讯项目。
[bilibili演示](https://www.bilibili.com/video/BV1ny411Y7DJ/?spm_id_from=333.999.0.0&vd_source=f9105755833a175d6cfb9429d860f415)

# 开发环境 
基于grpc-cpp，服务器在linux下;客户端在windows下，采用electron开发。

# 架构
![MyChat-Format]()

# 具体业务
## 登录业务
### 登录账号验证与状态同步
- 用户名密码校验
    - md5 + 混淆码
- 在线状态设置 + 路由状态更新
- 旧客户端账号下线
- 通知好友

### 登录数据准备
- 用户信息
- 
