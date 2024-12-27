# **这是一份HDU作业合集**    


1.贪吃蛇游戏   
-------------------
设计了吃有害物质变短,计分版等功能   

2.停车场 
-------------
以栈模拟停车场，以队列模拟车场外的便道，按照从终端读入的输入数据序列进行模拟管理。每一组输入数据包括三个数据项：汽车“到达”或“离去”信息、汽车牌照号码以及到达或离去的时刻。对每一组输入数据进行操作后的输出信息为：若是车辆到达，则输出汽车在停车场内或便道上的停车位置；若是车辆离去，则输出汽车在停车场内停留的时间和应交纳的费用（在便道上停留的时间不收费）。栈依顺序结构实现，队列以链表结构实现。

3.霍夫曼编码            
--------------------         
设计并实现一个写一个哈夫曼码的编/译码系统，系统功能包括：
（1）I：初始化（Initialization）。从终端读入字符集大小n，以及n个字符和n个权值，建立哈夫曼树，并将它存于文件hfmTree中；
（2）E：编码（Encoding）。利用已建好的哈夫曼树（如不在内存，则从文件hfmTree中读入），对文件ToBeTran中的正文进行编码，然后将结果存入文件CodeFile中；
（3）D：译码（Decoding）。利用已建好的哈夫曼树将文件CodeFile中的代码进行译码，结果存入文件TextFile中；
（4）P：印代码文件（Print）。将文件CodeFile以紧凑格式显示在终端上，每行50个代码。同时将此字符形式的编码文件写入文件CodePrin中；
（5）T：印哈夫曼树（Tree printing）。将已在内存中的哈夫曼树以直观的方式（树或凹入表形式）显示在终端上，同时将此字符形式的哈夫曼树写入文件TreePrint中。

4.校园导游图            
------------------------------------      
设计并实现一个校园导游程序，为来访的客人提供各种信息查询服务，具体包括：
（1）设计校园平面图，其中所含景点不少于10个。以图中顶点表示校内各景点，存放景点名称、代号、简介等信息：以边表示路径，存放路径长度等相关信息；
（2）为来访客人提供图中任意景点相关信息的查询；
（3）为来访客人提供途中任意景点的问路查询，即查询任意两个景点之间的一条最短的简单路径。

