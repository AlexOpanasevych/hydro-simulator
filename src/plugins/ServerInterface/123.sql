/*
Navicat SQL Server Data Transfer

Source Server         : sql
Source Server Version : 150000
Source Host           : 192.168.81.201:1433
Source Database       : ewarehouse3
Source Schema         : authorization

Target Server Type    : SQL Server
Target Server Version : 150000
File Encoding         : 65001

Date: 2020-11-04 18:49:00
*/


-- ----------------------------
-- Table structure for AccessRight
-- ----------------------------
DROP TABLE [AccessRight]
GO
CREATE TABLE [AccessRight] (
[id] int NOT NULL IDENTITY(1,1) ,
[editRequest] bit NOT NULL ,
[addRequest] bit NOT NULL ,
[confirmRequest] bit NOT NULL ,
[createNewUser] bit NOT NULL 
)


GO
DBCC CHECKIDENT(N'[AccessRight]', RESEED, 3)
GO

-- ----------------------------
-- Records of AccessRight
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [AccessRight] ON
GO
INSERT INTO [AccessRight] ([id], [editRequest], [addRequest], [confirmRequest], [createNewUser]) VALUES (N'1', N'1', N'1', N'1', N'1'), (N'2', N'0', N'0', N'0', N'0'), (N'3', N'1', N'1', N'0', N'0')
GO
GO
SET IDENTITY_INSERT [AccessRight] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for Permission
-- ----------------------------
DROP TABLE [Permission]
GO
CREATE TABLE [Permission] (
[id] int NOT NULL IDENTITY(1,1) ,
[permission] varchar(50) NOT NULL 
)


GO
DBCC CHECKIDENT(N'[Permission]', RESEED, 3)
GO

-- ----------------------------
-- Records of Permission
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [Permission] ON
GO
INSERT INTO [Permission] ([id], [permission]) VALUES (N'1', N'Master'), (N'2', N'User'), (N'3', N'GUEST')
GO
GO
SET IDENTITY_INSERT [Permission] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for Session
-- ----------------------------
DROP TABLE [Session]
GO
CREATE TABLE [Session] (
[id] int NOT NULL IDENTITY(1,1) ,
[name] int NOT NULL ,
[inputDate] varchar(50) NOT NULL ,
[macAddress] varchar(50) NOT NULL ,
[outputDare] varchar(50) NOT NULL 
)


GO

-- ----------------------------
-- Records of Session
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [Session] ON
GO
SET IDENTITY_INSERT [Session] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Table structure for Users
-- ----------------------------
DROP TABLE [Users]
GO
CREATE TABLE [Users] (
[id] int NOT NULL IDENTITY(1,1) ,
[name] varchar(50) NOT NULL ,
[password] nvarchar(300) NOT NULL ,
[permission] int NOT NULL ,
[accessRights] int NOT NULL 
)


GO
DBCC CHECKIDENT(N'[Users]', RESEED, 6)
GO

-- ----------------------------
-- Records of Users
-- ----------------------------
BEGIN TRANSACTION
GO
SET IDENTITY_INSERT [Users] ON
GO
INSERT INTO [Users] ([id], [name], [password], [permission], [accessRights]) VALUES (N'2', N'a', N'ʗ������1��#�M����|Nr��w���H�', N'1', N'1'), (N'5', N's', N':q�t�r��%�����\V��ϟ�?�]R���', N'2', N'2'), (N'6', N'w', N'P�!��?', N'3', N'3')
GO
GO
SET IDENTITY_INSERT [Users] OFF
GO
COMMIT TRANSACTION
GO

-- ----------------------------
-- Indexes structure for table AccessRight
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table AccessRight
-- ----------------------------
ALTER TABLE [AccessRight] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table Permission
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table Permission
-- ----------------------------
ALTER TABLE [Permission] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table Session
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table Session
-- ----------------------------
ALTER TABLE [Session] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table Users
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table Users
-- ----------------------------
ALTER TABLE [Users] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Foreign Key structure for table [Session]
-- ----------------------------
ALTER TABLE [Session] ADD FOREIGN KEY ([name]) REFERENCES [Users] ([id]) ON DELETE NO ACTION ON UPDATE NO ACTION
GO

-- ----------------------------
-- Foreign Key structure for table [Users]
-- ----------------------------
ALTER TABLE [Users] ADD FOREIGN KEY ([accessRights]) REFERENCES [AccessRight] ([id]) ON DELETE NO ACTION ON UPDATE NO ACTION
GO
ALTER TABLE [Users] ADD FOREIGN KEY ([permission]) REFERENCES [Permission] ([id]) ON DELETE NO ACTION ON UPDATE NO ACTION
GO
