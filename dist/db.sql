# World Wide Security - 2000
# Trojan Scanner
# Cassiano Aquino - (cassiano@wwsecurity.net)
# CVS Id  : $Id: db.sql,v 1.1.1.1 2001/03/26 05:44:25 cassiano Exp $
# CVS Log :
#           $Log: db.sql,v $
#           Revision 1.1.1.1  2001/03/26 05:44:25  cassiano
#           Imported into the CVS repository
#
CREATE TABLE log (
	ip varchar(30),
	what varchar(255),
	happen timestamp
);

CREATE TABLE counter (
	counter int(11) NOT NULL
);

INSERT INTO counter VALUES (22654);

CREATE TABLE language (
  language varchar(10),
  id int(4),
  content varchar(255)
);


INSERT INTO language VALUES ('en',0,'Eletronic Brain - Trojan Scanner v1.0');
INSERT INTO language VALUES ('en',1,'Remote IP');
INSERT INTO language VALUES ('en',2,'Trojans on DataBase');
INSERT INTO language VALUES ('en',3,'Trojan name');
INSERT INTO language VALUES ('en',4,'Installed');
INSERT INTO language VALUES ('en',5,'Trojans installed');
INSERT INTO language VALUES ('en',6,'Yes');
INSERT INTO language VALUES ('en',7,'No');
INSERT INTO language VALUES ('en',8,'Detected');
INSERT INTO language VALUES ('en',9,'Try to scan at least 2 times to be sure about the results, though, the conection speed or any kind of firewall can cause bad results.');
INSERT INTO language VALUES ('en',10,'Protocol');
INSERT INTO language VALUES ('en',11,'Port');
INSERT INTO language VALUES ('en',12,'go to');
INSERT INTO language VALUES ('en',13,'The UDP scan is pretty slowly, please wait, don\'t click the reload button');
INSERT INTO language VALUES ('en',14,'Visitor since 20/10/1999 14:57:15');
INSERT INTO language VALUES ('en',15,'http://www.ebrain.com.br/tsdicasUS.html');
INSERT INTO language VALUES ('pt',0,'Eletronic Brain - Trojan Scanner v1.0');
INSERT INTO language VALUES ('pt',1,'IP Remoto');
INSERT INTO language VALUES ('pt',2,'Trojans no Banco de Dados');
INSERT INTO language VALUES ('pt',3,'Nome da Trojan');
INSERT INTO language VALUES ('pt',4,'Instalada');
INSERT INTO language VALUES ('pt',5,'Trojans instaladas');
INSERT INTO language VALUES ('pt',6,'Sim');
INSERT INTO language VALUES ('pt',7,'N&atilde;o');
INSERT INTO language VALUES ('pt',8,'Detectado');
INSERT INTO language VALUES ('pt',9,'Tente scannear ao menos 2 vezes para ter certeza do resultado, pois, a velocidade da conex&atilde;o ou algum firewall pode afetar a checagem');
INSERT INTO language VALUES ('pt',10,'Protocolo');
INSERT INTO language VALUES ('pt',11,'Porta');
INSERT INTO language VALUES ('pt',12,'va em');
INSERT INTO language VALUES ('pt',13,'O scan de UDP eh lento, por favor aguarde, N&atilde;o clique no bot&atilde;o de atualizar');
INSERT INTO language VALUES ('pt',14,'Visitantes desde 20/10/1999 14:57:15');
INSERT INTO language VALUES ('pt',15,'http://www.ebrain.com.br/tsdicas.html');
INSERT INTO language VALUES ('en',16,'Checked');
INSERT INTO language VALUES ('pt',16,'Checado');
INSERT INTO language VALUES ('en',18,'Total of TCP Trojans');
INSERT INTO language VALUES ('pt',18,'Total de Trojans TCP');
INSERT INTO language VALUES ('en',17,'Total of UDP Trojans');
INSERT INTO language VALUES ('pt',17,'Total de Trojans UDP');
INSERT INTO language VALUES ('en',19,'TCP Trojans');
INSERT INTO language VALUES ('pt',19,'Trojans TCP');
INSERT INTO language VALUES ('en',20,'UDP Trojans');
INSERT INTO language VALUES ('pt',20,'Trojans UDP');
INSERT INTO language VALUES ('en',21,'Sorry cannot check UDP trojans in your
machine');
INSERT INTO language VALUES ('pt',21,'Desculpe mas n&atilde;o &eacute; possivel
checar sua maquina por Trojans UDP');
INSERT INTO language VALUES ('en',22,'Packet delay');
INSERT INTO language VALUES ('pt',22,'Demora do pacote');

CREATE TABLE trojanlist (
  id int(11) NOT NULL auto_increment,
  name varchar(50),
  port int(11),
  protocol int(11),
  signature varchar(255),
  PRIMARY KEY (id)
);


INSERT INTO trojanlist VALUES (1,'Masters Paradise',31,0,NULL);
INSERT INTO trojanlist VALUES (2,'unlg1.1',80,0,NULL);
INSERT INTO trojanlist VALUES (3,'Hidden Port 2.0',99,0,NULL);
INSERT INTO trojanlist VALUES (4,'BO Jammer Killah V',121,0,NULL);
INSERT INTO trojanlist VALUES (5,'Hackers Paradise',456,0,NULL);
INSERT INTO trojanlist VALUES (6,'Stealthspy / Phase0 / Netadmin / INI-Killer',555,0,NULL);
INSERT INTO trojanlist VALUES (7,'Attack FTP / Satans Backdoor',666,0,NULL);
INSERT INTO trojanlist VALUES (8,'Aimspy',777,0,NULL);
INSERT INTO trojanlist VALUES (9,'Der Spaeher 3',1000,0,NULL);
INSERT INTO trojanlist VALUES (10,'Silencer-Webex-Doly',1001,0,NULL);
INSERT INTO trojanlist VALUES (11,'Doly trojan v1.35',1010,0,NULL);
INSERT INTO trojanlist VALUES (12,'Doly Trojan',1011,0,NULL);
INSERT INTO trojanlist VALUES (13,'Doly trojan v1.5',1015,0,NULL);
INSERT INTO trojanlist VALUES (14,'Doly Trojan 1.6',1016,0,NULL);
INSERT INTO trojanlist VALUES (15,'Psyber Streaming Server',1024,0,NULL);
INSERT INTO trojanlist VALUES (16,'Incommand',1029,0,NULL);
INSERT INTO trojanlist VALUES (17,'Netspy',1033,0,NULL);
INSERT INTO trojanlist VALUES (18,'Blah 1.1',1042,0,NULL);
INSERT INTO trojanlist VALUES (19,'Mini Command 1.2',1050,0,NULL);
INSERT INTO trojanlist VALUES (20,'Wingate',1080,0,NULL);
INSERT INTO trojanlist VALUES (21,'Psyber Streaming Audio Server',1170,0,NULL);
INSERT INTO trojanlist VALUES (22,'Softwar',1207,0,NULL);
INSERT INTO trojanlist VALUES (23,'SubSeven or SubSeven 1.9',1243,0,NULL);
INSERT INTO trojanlist VALUES (24,'Voodoo Doll',1245,0,NULL);
INSERT INTO trojanlist VALUES (25,'Maverick\'s Matrix',1269,0,NULL);
INSERT INTO trojanlist VALUES (26,'Back Orifice DLL',1349,0,NULL);
INSERT INTO trojanlist VALUES (27,'FTP99CMP',1492,0,NULL);
INSERT INTO trojanlist VALUES (28,'Psyber Streaming Server',1509,0,NULL);
INSERT INTO trojanlist VALUES (29,'Shiva Burka',1600,0,NULL);
INSERT INTO trojanlist VALUES (30,'Spy Sender',1807,0,NULL);
INSERT INTO trojanlist VALUES (31,'ShockRave',1981,0,NULL);
INSERT INTO trojanlist VALUES (32,'Transcout',1999,0,NULL);
INSERT INTO trojanlist VALUES (33,'Backdoor',1999,0,NULL);
INSERT INTO trojanlist VALUES (34,'Insane Network 4',2000,0,NULL);
INSERT INTO trojanlist VALUES (35,'Der Spaeher 3 / Trojan Cow',2001,0,NULL);
INSERT INTO trojanlist VALUES (36,'Hack City Ripper Pro / Pass Ripper',2023,0,NULL);
INSERT INTO trojanlist VALUES (37,'Bugs',2115,0,NULL);
INSERT INTO trojanlist VALUES (38,'The Invasor',2140,0,NULL);
INSERT INTO trojanlist VALUES (39,'HVL Rat5',2283,0,NULL);
INSERT INTO trojanlist VALUES (40,'Striker',2565,0,NULL);
INSERT INTO trojanlist VALUES (41,'WinCrash2',2583,0,NULL);
INSERT INTO trojanlist VALUES (42,'The Prayer2',2716,0,NULL);
INSERT INTO trojanlist VALUES (43,'Phineas Phucker',2801,0,NULL);
INSERT INTO trojanlist VALUES (44,'WinCrash',3024,0,NULL);
INSERT INTO trojanlist VALUES (45,'Masters Paradise',3129,0,NULL);
INSERT INTO trojanlist VALUES (46,'Deep Throat / Invasor',3150,0,NULL);
INSERT INTO trojanlist VALUES (47,'Total Eclypse 1.0',3791,0,NULL);
INSERT INTO trojanlist VALUES (48,'Psyber Streaming Server',4000,0,NULL);
INSERT INTO trojanlist VALUES (49,'WinCrash',4092,0,NULL);
INSERT INTO trojanlist VALUES (50,'Schoolbus 1.0',4321,0,NULL);
INSERT INTO trojanlist VALUES (51,'FileNail',4567,0,NULL);
INSERT INTO trojanlist VALUES (52,'ICQTrojan',4950,0,NULL);
INSERT INTO trojanlist VALUES (53,'Socket 23 / Sockets De Troie',5000,0,NULL);
INSERT INTO trojanlist VALUES (54,'Sockets De Troie',5001,0,NULL);
INSERT INTO trojanlist VALUES (55,'OOTLT / OOTLT Cart',5011,0,NULL);
INSERT INTO trojanlist VALUES (56,'Net Metropolitan 1.0 or 1.04',5031,0,NULL);
INSERT INTO trojanlist VALUES (57,'NetMetro 1.0',5031,0,NULL);
INSERT INTO trojanlist VALUES (58,'Net Metropolitan 1.04',5032,0,NULL);
INSERT INTO trojanlist VALUES (59,'Blade Runner',5042,0,NULL);
INSERT INTO trojanlist VALUES (60,'FireHotcker',5321,0,NULL);
INSERT INTO trojanlist VALUES (61,'Back Construction 1.2 1.5 / Blade Runner',5400,0,NULL);
INSERT INTO trojanlist VALUES (62,'Blade Runner',5401,0,NULL);
INSERT INTO trojanlist VALUES (63,'Illusion Mailer',5521,0,NULL);
INSERT INTO trojanlist VALUES (64,'XTCP2',5550,0,NULL);
INSERT INTO trojanlist VALUES (65,'RoboHack',5569,0,NULL);
INSERT INTO trojanlist VALUES (66,'PC Crasher',5637,0,NULL);
INSERT INTO trojanlist VALUES (67,'WinCrash',5714,0,NULL);
INSERT INTO trojanlist VALUES (68,'WinCrash',5742,0,NULL);
INSERT INTO trojanlist VALUES (69,'The Thing',6000,0,NULL);
INSERT INTO trojanlist VALUES (70,'TCPShell (*NIX Backdoor)',6666,0,NULL);
INSERT INTO trojanlist VALUES (71,'Host Control / Vampire 1.0',6669,0,NULL);
INSERT INTO trojanlist VALUES (72,'Deep Throat 1 or 2 or 3.x',6670,0,NULL);
INSERT INTO trojanlist VALUES (73,'Deep Throat / SubSeven',6711,0,NULL);
INSERT INTO trojanlist VALUES (74,'SubSeven access',6776,0,NULL);
INSERT INTO trojanlist VALUES (75,'Delta Source',6883,0,NULL);
INSERT INTO trojanlist VALUES (76,'Shitheep',6912,0,NULL);
INSERT INTO trojanlist VALUES (77,'Shitheep Danny',6913,0,NULL);
INSERT INTO trojanlist VALUES (78,'Indoctrination',6939,0,NULL);
INSERT INTO trojanlist VALUES (79,'Gatecrasher',6969,0,NULL);
INSERT INTO trojanlist VALUES (80,'Gatecrasher',6970,0,NULL);
INSERT INTO trojanlist VALUES (81,'Net Monitor',7300,0,NULL);
INSERT INTO trojanlist VALUES (82,'Net Monitor',7301,0,NULL);
INSERT INTO trojanlist VALUES (83,'Net Monitor',7302,0,NULL);
INSERT INTO trojanlist VALUES (84,'Net Monitor',7303,0,NULL);
INSERT INTO trojanlist VALUES (85,'Net Monitor',7304,0,NULL);
INSERT INTO trojanlist VALUES (86,'Net Monitor',7305,0,NULL);
INSERT INTO trojanlist VALUES (87,'Net Monitor',7306,0,NULL);
INSERT INTO trojanlist VALUES (88,'Net Monitor',7307,0,NULL);
INSERT INTO trojanlist VALUES (89,'Net Monitor',7308,0,NULL);
INSERT INTO trojanlist VALUES (90,'Net Monitor',7309,0,NULL);
INSERT INTO trojanlist VALUES (91,'ICQ Killer',7789,0,NULL);
INSERT INTO trojanlist VALUES (92,'Back Orifice 2000',8787,0,NULL);
INSERT INTO trojanlist VALUES (93,'Hack Office Armageddon',8879,0,NULL);
INSERT INTO trojanlist VALUES (94,'InCommand 1.0',9400,0,NULL);
INSERT INTO trojanlist VALUES (95,'Portal Of Doom',9872,0,NULL);
INSERT INTO trojanlist VALUES (96,'Portal of Doom',9875,0,NULL);
INSERT INTO trojanlist VALUES (97,'INI-Killer',9889,0,NULL);
INSERT INTO trojanlist VALUES (98,'The Prayer 1.x',9999,0,NULL);
INSERT INTO trojanlist VALUES (99,'BrainSpy',10101,0,NULL);
INSERT INTO trojanlist VALUES (100,'Brain Spy',10101,0,NULL);
INSERT INTO trojanlist VALUES (101,'Coma',10607,0,NULL);
INSERT INTO trojanlist VALUES (102,'Ambush',10666,0,NULL);
INSERT INTO trojanlist VALUES (103,'Linux mountd Backdoor',10752,0,NULL);
INSERT INTO trojanlist VALUES (104,'Sennaspy',11000,0,NULL);
INSERT INTO trojanlist VALUES (105,'Senna Spy Trojans',11000,0,NULL);
INSERT INTO trojanlist VALUES (106,'Progenic Trojan',11223,0,NULL);
INSERT INTO trojanlist VALUES (107,'Gjamer',12076,0,NULL);
INSERT INTO trojanlist VALUES (108,'hack\'99 keylogger',12223,0,NULL);
INSERT INTO trojanlist VALUES (109,'Netbus 1.x/ GabanBus',12345,0,NULL);
INSERT INTO trojanlist VALUES (110,'Netbus 1.x/ GabanBus',12346,0,NULL);
INSERT INTO trojanlist VALUES (111,'Whack-a-mole',12361,0,NULL);
INSERT INTO trojanlist VALUES (112,'Whack-a-mole',12362,0,NULL);
INSERT INTO trojanlist VALUES (113,'Eclipse 2000',12701,0,NULL);
INSERT INTO trojanlist VALUES (114,'Kuang2 The Virus',13700,0,NULL);
INSERT INTO trojanlist VALUES (115,'Mosucker',16484,0,NULL);
INSERT INTO trojanlist VALUES (116,'Priority / Portal Of Doom',16969,0,NULL);
INSERT INTO trojanlist VALUES (117,'Priotrity',16969,0,NULL);
INSERT INTO trojanlist VALUES (118,'Kuang2 theVirus',17300,0,NULL);
INSERT INTO trojanlist VALUES (119,'Millenium',20000,0,NULL);
INSERT INTO trojanlist VALUES (120,'NetBus2 Pro',20034,0,NULL);
INSERT INTO trojanlist VALUES (121,'Chupacabra',20203,0,NULL);
INSERT INTO trojanlist VALUES (122,'Logged!',20203,0,NULL);
INSERT INTO trojanlist VALUES (123,'Bla',20331,0,NULL);
INSERT INTO trojanlist VALUES (124,'GirlFriend or Schwindler 1.82',21554,0,NULL);
INSERT INTO trojanlist VALUES (125,'Prosiak 0.47',22222,0,NULL);
INSERT INTO trojanlist VALUES (126,'UglyFtp or WhackJob',23456,0,NULL);
INSERT INTO trojanlist VALUES (127,'Delta',26274,0,NULL);
INSERT INTO trojanlist VALUES (128,'Trin00',27665,0,NULL);
INSERT INTO trojanlist VALUES (129,'The Unexplained',29891,0,NULL);
INSERT INTO trojanlist VALUES (130,'AOL Trojan1.1',30029,0,NULL);
INSERT INTO trojanlist VALUES (131,'NetSphere',30100,0,NULL);
INSERT INTO trojanlist VALUES (132,'Trojan Spirit 2001a',30133,0,NULL);
INSERT INTO trojanlist VALUES (133,'NetSphere Final 131337',30133,0,NULL);
INSERT INTO trojanlist VALUES (134,'Socket 25',30303,0,NULL);
INSERT INTO trojanlist VALUES (135,'Socket 23',30303,0,NULL);
INSERT INTO trojanlist VALUES (136,'Kuang',30999,0,NULL);
INSERT INTO trojanlist VALUES (137,'Deep Back Orifice',31338,0,NULL);
INSERT INTO trojanlist VALUES (138,'NetSpy DK',31339,0,NULL);
INSERT INTO trojanlist VALUES (139,'Schwindler',31554,0,NULL);
INSERT INTO trojanlist VALUES (140,'BOWhack',31666,0,NULL);
INSERT INTO trojanlist VALUES (141,'Hack\'a\'tack',31787,0,NULL);
INSERT INTO trojanlist VALUES (142,'Acid Battery 1.0',32418,0,NULL);
INSERT INTO trojanlist VALUES (143,'Trojan Spirit 2001a',33911,0,NULL);
INSERT INTO trojanlist VALUES (144,'BigGluck / Tiny Telnet Server',34324,0,NULL);
INSERT INTO trojanlist VALUES (145,'YAT - Yet Another Trojan',37651,0,NULL);
INSERT INTO trojanlist VALUES (146,'The Spy',40412,0,NULL);
INSERT INTO trojanlist VALUES (147,'Masters Paradise',40421,0,NULL);
INSERT INTO trojanlist VALUES (148,'Masters Paradise',40422,0,NULL);
INSERT INTO trojanlist VALUES (149,'Masters Paradise',40423,0,NULL);
INSERT INTO trojanlist VALUES (150,'Masters Paradise',40426,0,NULL);
INSERT INTO trojanlist VALUES (151,'Schoolbus 1.6 / 2.0',43210,0,NULL);
INSERT INTO trojanlist VALUES (152,'Prosiak',44444,0,NULL);
INSERT INTO trojanlist VALUES (153,'Delta',47262,0,NULL);
INSERT INTO trojanlist VALUES (154,'Online Keylogger',49301,0,NULL);
INSERT INTO trojanlist VALUES (155,'Sockets de Troie',50505,0,NULL);
INSERT INTO trojanlist VALUES (156,'Fore Schwindler',50766,0,NULL);
INSERT INTO trojanlist VALUES (157,'Fore Schwindler / Remote Windows Shutdown',50776,0,NULL);
INSERT INTO trojanlist VALUES (158,'Remote Windows Shutdown',53001,0,NULL);
INSERT INTO trojanlist VALUES (159,'Back Orifice 2000 tcp',54320,0,NULL);
INSERT INTO trojanlist VALUES (160,'Schoolbus 1.6 / 2.0',54321,0,NULL);
INSERT INTO trojanlist VALUES (161,'Netraider',57341,0,NULL);
INSERT INTO trojanlist VALUES (162,'Tele Commando',61466,0,NULL);
INSERT INTO trojanlist VALUES (163,'Devil 1.03',65000,0,NULL);
INSERT INTO trojanlist VALUES (164,'SheetHeep',69123,0,NULL);
INSERT INTO trojanlist VALUES (165,'Hackers Paradise',31,1,NULL);
INSERT INTO trojanlist VALUES (166,'Incommand',1029,1,NULL);
INSERT INTO trojanlist VALUES (167,'Ultors Trojan',1234,1,NULL);
INSERT INTO trojanlist VALUES (168,'Rat Backdoor',2989,1,NULL);
INSERT INTO trojanlist VALUES (169,'Portal Of Doom',3700,1,NULL);
INSERT INTO trojanlist VALUES (170,'Remote Grab',7000,1,NULL);
INSERT INTO trojanlist VALUES (171,'Trin00',27444,1,NULL);
INSERT INTO trojanlist VALUES (172,'Trin00',31335,1,NULL);
INSERT INTO trojanlist VALUES (173,'Back Oriffice',31337,1,NULL);
INSERT INTO trojanlist VALUES (174,'Deep Back Orifice',31338,1,NULL);
INSERT INTO trojanlist VALUES (175,'Prosiak',33333,1,NULL);
INSERT INTO trojanlist VALUES (176,'Back Orifice 2000 udp',54321,1,NULL);

