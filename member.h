#pragma once
#include<iostream>
#include<string>
#include<ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
#include <ctype.h>
#include"warehouseoprate.h"
#include<tchar.h>
#include<Windows.h>
using namespace std;
#define SIZE 100//最大用户容量
class department//先从数据中搜索该人员编号，查找是哪一部门的，//1是主管，2是前台，3是仓库管理员
{
public:
	string member_name = "";//人员名称
	string member_number = "";//人员编号
	string age = "";//年龄
	string sex = "";//性别

	
	department() = default;


	department(const string& member_name, const string& member_number, const string& age, const string& sex)
		: member_name(member_name), member_number(member_number), age(age), sex(sex)
	{
	}
};
class manager :public department
{
public:
	string level = "1";//1是主管，2是前台，3是仓库管理员
	manager(const string& member_name, const string& member_number, const string& age, const string& sex);
	void read_warehouse_form();
	manager() = default;
};
class counter :public department//在user类中把counter作为成员对象。在文件比对密码正确后，跳转，顺便赋值counter的成员属性。
	//然后就能在通过user调用counter的成员函数（创建订单对象等）
{
public:
	string level = "2";//1是主管，2是前台，3是仓库管理员
	counter(const string& member_name, const string& member_number, const string& age, const string& sex);
	void read_perchase_form();//入库订单
	counter() = default;
};
class warehouser :public department
{
public:
	string level = "3";//1是主管，2是前台，3是仓库管理员
	//管理员权限
	warehouser(const string& member_name, const string& member_number, const string& age, const string& sex);
	void check_warehouse_form();
	void motify_warehouse_form();
	warehouser() = default;
};
class User
{
public:
	string num = "";//账号
	string level = "0";//等级
	string password = "";//密码
	department temp;//多态
	User() {};
	void Registers();//注册
	void Login();//登录
	void save();//保存
	void read();//读取
	void csv_read(int i, ifstream& _csvInput);
	void func_manager(int i);
	void func_counter(int i);
	void func_warehouser(int i);
};
int scount = 0;//用作储存当前已注册用户数
User user[SIZE];
class order {
public:
	goods order_goods;
	string order_number;
	string order_operate;
	string time;
	double amount;
	department user;
	order(const goods& order_goods, const string& order_number, string order_operate,department user, double amount)
		: order_goods(order_goods), order_number(order_number), order_operate(order_operate), user(user), amount(amount)
	{
		time = rtime();
	}
	void display_order()
	{
		//cout << "订单编号：时间 商品编号： 供货商  描述  种类  容量单位 进价  售价  数量：  出入库 负责人编号  负责人 " << endl;
		cout << "订单编号：" << order_number << "\t" << "时间：" << time << "\t" << "商品编号：" << order_goods.goods_number << "\t" << "供货商：" << order_goods.supplier_name << "\t" << "描述：" << order_goods.goods_description << "\t" << "种类：" << order_goods.type << "\t"
			<< "进价：" << order_goods.retall_price << "\t" << "售价：" << order_goods.sale_price << "\t" << "数量：" << amount <<
			"\t" << "出入库：" << order_operate << "\t" << "负责人编号：" << user.member_number << "\t" << "负责人：" << user.member_name << "\t" << endl;
	}

	order() = default;

	order(const goods& order_goods, const string& order_number, const string& order_operate,  const string& time, double amount, const department& user)
		: order_goods(order_goods), order_number(order_number), order_operate(order_operate), time(time), amount(amount), user(user)
	{
	}
};
void Supplier_Orders(department user)
{
	//cout << user.member_name << endl;
	cout << "输入订单个数：";
	int total_order;
	cin >> total_order;
	cout << endl;
	for (int order_number = 0; order_number < total_order; order_number++)
	{//
		string goods_num, suppiler, des, type;
		int mea;
		double retall, sale, amount;
		cout << endl << "依次输入商品编号，供货商，描述，种类，容量单位（毫升：1，升：2），进价，售价, 数量" << endl;
		cin >> goods_num >> suppiler >> des >> type >> mea >> retall >> sale >> amount;
		goods temp(goods_num, suppiler, des, type, mea, retall, sale);
		string order_number1 = num_time() + goods_num;
		order order_temp(temp, order_number1, "入库", user, amount);
		if (Storage(temp, amount))
		{
			cout << "入库成功！" << endl;
		ofstream ofile("order.csv", ios::app);
			ofile << order_temp.order_number << ',' << order_temp.time << ',' << order_temp. order_goods.goods_number << ',' << order_temp.order_goods.supplier_name << ',' 
				<< order_temp.order_goods.goods_description << ',' << order_temp.order_goods.type << ','<<to_string(order_temp.order_goods.measurement)<<','
			<< to_string(order_temp.order_goods.retall_price) << ',' << to_string(order_temp.order_goods.sale_price) << ',' << to_string(order_temp.amount) <<
			',' << "入库" << ',' << user.member_number << ',' << user.member_name  << endl;
			ofile.close();
			cout << "正在入库中.....";
				//sleep(1000);
		}
			else
				cout << "入库失败"<<endl;
	}
	cout << "finish" << endl;
}
void Supplier_Order_csv(department user)
{
	string filepath;
	cout << "输入需读取文件的路径：";
		cin >> filepath;
		string filename;
		cout << " 输入需要读取的文件名：";
		cin >> filename;
		cout << endl;
		filepath += filename;
		//cout << filepath;
		string fileop = "D:\\Datestruct\\data\\" + filename;
		MoveFile(_T(filepath.c_str()), _T(fileop.c_str()));
		ifstream ifile(filename, ios::in);
		if (!ifile.is_open())
			cout << "open fail" << endl;
		else
		{
			string line;
			string temp[9];
			int temp_i = 1;
			while (getline(ifile, line))
			{
				stringstream ss(line);
				string str;
				for (int i = 0; i < 9; i++)
				{
					getline(ss, str, ',');
					temp[i] = str;
				}
					goods temp_good(temp[2],temp[1],temp[3],temp[4],stoi(temp[8]),stod(temp[5]),stod(temp[6]));
					string num_order = "R";
					num_order += num_time() + temp[2]+to_string(temp_i);
					order order_temp(temp_good, num_order, "入库", temp[0], stod(temp[7]),user);
					
					if (Storage(temp_good, stod(temp[7])))
					{
						cout << "入库成功！" << endl;
					ofstream ofile("order.csv", ios::app);
					ofile << order_temp.order_number << ',' << order_temp.time << ',' << order_temp.order_goods.goods_number << ',' << order_temp.order_goods.supplier_name << ','
						<< order_temp.order_goods.goods_description << ',' << order_temp.order_goods.type << ',' << to_string(order_temp.order_goods.measurement) << ','
						<< to_string(order_temp.order_goods.retall_price) << ',' << to_string(order_temp.order_goods.sale_price) << ',' << to_string(order_temp.amount) <<
						',' << "入库" << ',' << user.member_number << ',' << user.member_name << endl;
					ofile.close();
					}
				else
					cout << "入库失败" << endl;
					
					temp_i++;
			}
			ifile.close();
		}

}
void Distributors(department user)
{
	cout << "输入订单个数：";
	int total_order;
	cin >> total_order;
	cout << endl;
	for (int order_number = 0; order_number < total_order; order_number++)
	{
		getgoods_index();
		cout << "输入商品编号：";
			string goods_num;
		cin >> goods_num;
		goods temp;
		FILE* fp;
		if (!(fp = fopen("warehouse.dat", "rb+")))
			cout << "create warehouse fail" << endl;
		fseek(fp, 0, SEEK_END);
		int sizelong = ftell(fp);
		int filenum = sizelong / sizeof(ware_goods);
		rewind(fp);
		ware_goods* index = new ware_goods[filenum];
		fread(index, sizeof(ware_goods), filenum, fp);
		rewind(fp);
		for (int i = 0; i < 200; i++)
		{
           if (index[i].goods_number == goods_num)
			{
			   temp=goods(index[i]);
			  
				fclose(fp); 
				break;
			}
		   if (index[i].goods_number == " ")
		   {    
			   cout << "无该商品" << endl;///
			   fclose(fp);
			   break;
		  }
		}
		cout << endl << "输入商品数量：";
			double amount;
		cin >> amount;
		cout << endl;
		order order_temp(temp, num_time()+temp.goods_number, "出库", user, amount);
		if (delivery(temp, amount))
		{
			cout << "出库成功！";
				ofstream ofile("order.csv", ios::app);
			ofile << order_temp.order_number << ',' << order_temp.time << ',' << order_temp.order_goods.goods_number << ',' << order_temp.order_goods.supplier_name << ',' << order_temp.order_goods.goods_description << ',' << order_temp.order_goods.type << ',' <<
				to_string( order_temp.order_goods.measurement)
				<< to_string(order_temp.order_goods.retall_price) << ',' << to_string(order_temp.order_goods.sale_price) << ',' << to_string(order_temp.amount) <<
				',' << "出库" << ',' << user.member_number << ',' << user.member_name << ',' << endl;
			ofile.close();
		}
		else
			cout << "出库失败！" << endl;
	}
}
void Distributors_csv(department user)
{
	string filepath;
	cout << "输入需读取文件的路径：";
	cin >> filepath;
	string filename;
	cout << " 输入需要读取的文件名：";
	cin >> filename;
	cout << endl;
	filepath += filename;
	string fileop = "D:\\Datestruct\\data\\" + filename;
	MoveFile(_T(filepath.c_str()), _T(fileop.c_str()));
	ifstream ifile(filename, ios::in);
	if (!ifile.is_open())
		cout << "open fail" << endl;
	else
	{
		string line;
		string temp[9];
		int temp_i = 1;
		while (getline(ifile, line))
		{
			stringstream ss(line);
			string str;
			for (int i = 0; i < 9; i++)
			{
				getline(ss, str, ',');
				temp[i] = str;
			}
			goods temp_good(temp[2], temp[1], temp[3], temp[4], stoi(temp[8]), stod(temp[5]), stod(temp[6]));
			string num_order = "C";
			num_order += num_time() + temp[2] + to_string(temp_i);
			order order_temp(temp_good, num_order, "出库", temp[0], stod(temp[7]), user);
			if (delivery(temp_good, stod(temp[7])))
			{
				cout << "出库成功！" << endl;
				ofstream ofile("order.csv", ios::app);
				ofile << order_temp.order_number << ',' << order_temp.time << ',' << order_temp.order_goods.goods_number << ',' << order_temp.order_goods.supplier_name << ','
					<< order_temp.order_goods.goods_description << ',' << order_temp.order_goods.type << ',' << to_string(order_temp.order_goods.measurement) << ','
					<< to_string(order_temp.order_goods.retall_price) << ',' << to_string(order_temp.order_goods.sale_price) << ',' << to_string(order_temp.amount) <<
					',' << "出库" << ',' << user.member_number << ',' << user.member_name << endl;
				ofile.close();
			}
			else
				cout << "入库失败" << endl;

			temp_i++;
		}
		ifile.close();
	}
}
void read_order() {

	ifstream ifile("order.csv", ios::in);
	if (!ifile.is_open())
		cout << "open fail" << endl;
	else
	{
		string line;
		string temp[13];
		while (getline(ifile, line))
		{
			stringstream ss(line);
			string str;
			for (int i = 0; i < 13; i++)
			{
				getline(ss, str, ',');
				temp[i] = str;
			}
			goods temp_good(temp[2], temp[3], temp[4], temp[5], stoi(temp[6]), stod(temp[7]), stod(temp[8]));
			department user(temp[12], temp[11], " ", " ");
			order order_temp(temp_good, temp[0], temp[10], temp[1], stod(temp[9]),user);
			order_temp.display_order();
		}
		ifile.close();
	}
}
void read_order_by_head(string temp_num)
{   
	
	ifstream ifile("order.csv", ios::in);
	if (!ifile.is_open())
		cout << "open fail" << endl;
	else
	{
		string line;
		string temp[13];

		while (getline(ifile, line))
		{
			stringstream ss(line);
			string str;
			for (int i = 0; i < 13; i++)
			{
				getline(ss, str, ',');
				temp[i] = str;
			}
			if (temp[11] == temp_num)
			{
				goods temp_good(temp[2], temp[3], temp[4], temp[5], stoi(temp[6]), stod(temp[7]), stod(temp[8]));
				department user(temp[12], temp[11], " ", " ");
				order order_temp(temp_good, temp[0], temp[10], temp[1], stod(temp[9]), user);
				order_temp.display_order();
			}
			
		}
		ifile.close();
	}
}
void read_order_by_goods()
{
	cout << "输入商品编号：" << endl;
	string temp_num;
	cin >> temp_num;
	cout << endl;
	ifstream ifile("order.csv", ios::in);
	if (!ifile.is_open())
		cout << "open fail" << endl;
	else
	{
		string line;
		string temp[13];
		while (getline(ifile, line))
		{
			stringstream ss(line);
			string str;
			for (int i = 0; i < 13; i++)
			{
				getline(ss, str, ',');
				temp[i] = str;
			}
			if (temp[2] == temp_num)
			{
				goods temp_good(temp[2], temp[3], temp[4], temp[5], stoi(temp[6]), stod(temp[7]), stod(temp[8]));
				department user(temp[12], temp[11], " ", " ");
				order order_temp(temp_good, temp[0], temp[10], temp[1], stod(temp[9]), user);
				order_temp.display_order();
			}
		}
		ifile.close();
}
void read_order_by_operation(bool temp)
{
	string op;
	if (temp)
		op = "入库";
	else
		op = "出库";
	ifstream ifile("order.csv", ios::in);
	if (!ifile.is_open())
		cout << "open fail" << endl;
	else
	{
		string line;
		string temp[13];
		while (getline(ifile, line))
		{
			stringstream ss(line);
			string str;
			for (int i = 0; i < 13; i++)
			{
				getline(ss, str, ',');
				temp[i] = str;
			}
			if (temp[10] == op)
			{
				goods temp_good(temp[2], temp[3], temp[4], temp[5], stoi(temp[6]), stod(temp[7]), stod(temp[8]));
				department user(temp[12], temp[11], " ", " ");
				order order_temp(temp_good, temp[0], temp[10], temp[1], stod(temp[9]), user);
				order_temp.display_order();
			}
		}
		ifile.close();
	}
}
void read_order_by_num()
{
	cout << "输入订单编号：" << endl;
	string temp_num;
	cin >> temp_num;
	cout << endl;
	ifstream ifile("order.csv", ios::in);
	if (!ifile.is_open())
		cout << "open fail" << endl;
	else
	{
		string line;
		string temp[13];
		while (getline(ifile, line))
		{
			stringstream ss(line);
			string str;
			for (int i = 0; i < 13; i++)
			{
				getline(ss, str, ',');
				temp[i] = str;
			}
			if (temp[0] == temp_num)
			{
				goods temp_good(temp[2], temp[3], temp[4], temp[5], stoi(temp[6]), stod(temp[7]), stod(temp[8]));
				department user(temp[12], temp[11], " ", " ");
				order order_temp(temp_good, temp[0], temp[10], temp[1], stod(temp[9]), user);
				order_temp.display_order();
			}
		}
		ifile.close();
	}

}
void read_by_month()
{
	cout << "输入需查询的年月份：";
	string temp_month;
	cin >> temp_month;
	cout << endl;
	double retall = 0;
	double sale = 0;

	ifstream ifile("order.csv", ios::in);
	if (!ifile.is_open())
		cout << "open fail" << endl;
	else
	{
		string line;
		string temp[13];
		while (getline(ifile, line))
		{
			stringstream ss(line);
			string str;
			for (int i = 0; i < 13; i++)
			{
				getline(ss, str, ',');
				temp[i] = str;
			}
			if (temp[1].find(temp_month)!=temp[1].npos)
			{
				if (temp[10] == "出库")
					sale += stod(temp[9]) * stod(temp[8]);
				if(temp[10]=="入库")
					retall+= stod(temp[9]) * stod(temp[7]);
			}
		}
		ifile.close();
		cout << temp_month << "盈利：" << sale << " 元 " << "支出：" << retall << " 元" << endl;
	}
}
void read_by_year()
{
	cout << "输入需查询的年份：";
	int  temp_month;
	cin >> temp_month;
	cout << endl;
	double retall = 0;
	double sale = 0;
	string op;
	op += to_string(temp_month);
	ifstream ifile("order.csv", ios::in);
	if (!ifile.is_open())
		cout << "open fail" << endl;
	else
	{
		string line;
		string temp[13];
		while (getline(ifile, line))
		{
			stringstream ss(line);
			string str;
			for (int i = 0; i < 13; i++)
			{
				getline(ss, str, ',');
				temp[i] = str;
			}
			if (temp[1].find(op) != temp[1].npos)
			{
				if (temp[10] == "出库")
					sale += stod(temp[9]) * stod(temp[8]);
				if (temp[10] == "入库")
					retall += stod(temp[9]) * stod(temp[7]);
			}
		}
		ifile.close();
		cout << op << "盈利：" << sale << " 元 " << "支出：" << retall << " 元" << endl;
	}
}
void predit()
{
	
	double sale = 0;
		FILE* fp;
		if (!(fp = fopen("warehouse.dat", "rb+")))
			cout << "create warehouse fail" << endl;
		fseek(fp, 0, SEEK_END);
		int sizelong = ftell(fp);
		int filenum = sizelong / sizeof(ware_goods);
		rewind(fp);
		ware_goods* index = new ware_goods[filenum];
		fread(index, sizeof(ware_goods), filenum, fp);
		cout << "商品编号\t" << "供货商\t" << "商品描述\t" << "数量\t" << endl;
		for (int i = 0; i < 100; i++)
		{
			if (index[i].goods_number != " ")
			{
				goods temp(index[i].goods_number, "llp", "fg", "beer", 1, 2, 2);
				int mid = delivery_index(temp);
				double count = 0;
				while (index[mid].next != -1)
				{
					count += index[mid].amount;
					mid = index[mid].next;
				}
				count += index[mid].amount;
				cout << index[mid].goods_number << "\t" << index[mid].supplier_name << "\t" << index[mid].goods_description << "\t" << count << endl;
				sale += index[mid].sale_price * count;
			}
		}
		fclose(fp);
		cout << "预计再盈利 " << sale << " 元"<<endl;
	
}
void User::func_manager(int i)
{
	//打印功能选择界面
	system("cls");
	int flag = 0;
	manager m;
	m.member_name = user[i].temp.member_name; m.member_number = user[i].temp.member_number; m.age = user[i].temp.age; m.sex = user[i].temp.sex;
	while (!flag)
	{
		cout << endl;
		cout << "\t\t\t┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
		cout << "\t\t\t┃		" << m.member_name << "主管你好：                                                               ┃\n";
		cout << "\t\t\t┃   1.查看历史所有订单 2.查看某负责人的历史订单 3.查看某商品的历史订单 4.查看所有出库订单  ┃\n";
		cout << "\t\t\t┃   5.查看所有入库订单 6.查看订单号 7.查看月营收情况  8.查看年营收  9.预测未来营收  10.退出┃\n";
		cout << "\t\t\t┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
		cout << "\t\t\t【系统提示】请输入你的选择：";
		int choose;
		cin >> choose;
		string temp_num1, temp_name1;
		switch (choose)
		{
		case 1:
		//	m.read_warehouse_form();
			read_order();
			break;
		case 2:
			cout << "输入责任人编号，姓名 :";
			cin >> temp_num1 >> temp_name1;
			cout << endl;
				read_order_by_head(temp_num1);
				break;
		case 3:
			read_order_by_goods();
			break;
		case 4:
			read_order_by_operation(false);
			break;
		case 5:
			read_order_by_operation(true);
			break;
		case 6:
			read_order_by_num();
			break;
		case 7:
			read_by_month();
			break;
		case 8:
			read_by_year();
			break;
		case 9:
			predit();
			break;
		case 10:
			flag = 1;
			break;
		default:
			cout << "\t\t\t请重新输入：";
			break;
		}
	}
}
void User::func_counter(int i)
{
	//打印功能选择界面
	system("cls");
	int flag = 0;
	counter c;
	c.member_name = user[i].temp.member_name; c.member_number = user[i].temp.member_number; c.age = user[i].temp.age; c.sex = user[i].temp.sex;
	while (!flag)
	{
		cout << endl;
		cout << "\t\t\t┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
		cout << "\t\t\t┃		" << c.member_name << "你好：								 ┃\n";
		cout << "\t\t\t┃      1.手动录入进货订单 2通过文件导入订单 3.查看历史入库订单 4.查看本人入库订单 5.退出 ┃\t\n";
		cout << "\t\t\t┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
		cout << "\t\t\t【系统提示】请输入你的选择：";
		int choose;

		cin >> choose;
		switch (choose)
		{
		case 1:

			//c.read_perchase_form();
			Supplier_Orders(user[i].temp);
			break;
		case 2:
			Supplier_Order_csv(user[i].temp);
			break;
		case 3:
			read_order_by_operation(true);
			break;
		case 4:
			read_order_by_head(user[i].temp.member_number);
			break;
		case 5:
			flag = 1;
			break;
		default:
			cout << "\t\t\t请重新输入：";
			break;
		}
	}
}
void User::func_warehouser(int i)
{
	//打印功能选择界面
	system("cls");
	int flag = 0;
	warehouser w;
	w.member_name = user[i].temp.member_name; w.member_number = user[i].temp.member_number; w.age = user[i].temp.age; w.sex = user[i].temp.sex;
	while (!flag)
	{
		cout << endl;
		cout << "\t\t\t┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
		cout << "\t\t\t┃		" << w.member_name << "管理员你好：				       ┃\n";
		cout << "\t\t\t┃       1.查看历史出库订单 2.录入出库订单 3.通过文件导入订单   ┃\n";
		cout << "\t\t\t┃       4.查看本人出库订单 5.查看仓库库存 6.新建仓库 7.退出    ┃\n";
		cout << "\t\t\t┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
		cout << "\t\t\t【系统提示】请输入你的选择：";
		int choose;
		cin >> choose;
		switch (choose)
		{
		case 1:
		//	w.check_warehouse_form();
			read_order_by_operation(false);
			break;
		case 2:
			Distributors(user[i].temp);
			break;
		case 3:
			cout << user[i].temp.member_name << endl;
			Distributors_csv(user[i].temp);
			break;
		case 4:
			read_order_by_head(user[i].temp.member_number);
			break;
		case 5:
			getgoods_index();
			break;
		case 6:
			setwarehouse();
			break;
		case 7:
			flag = 1;
			break;
		default:
			cout << "\t\t\t请重新输入：";
			break;
		}
	}
}
//保存
void User::save()
{
	ofstream ofile;
	ofile.open("user.csv");

	for (int i = 0; i < scount; i++)//将scount个账户信息写入文件
	{
		ofile << user[i].num << ','
			<< user[i].level << ','
			<< user[i].password << ','
			<< user[i].temp.member_name << ','
			<< user[i].temp.member_number << ','
			<< user[i].temp.age << ','
			<< user[i].temp.sex << ','
			<< endl;
	}
	ofile.close();
}
//读取
void User::csv_read(int i, ifstream& _csvInput)
{
	string _Oneline;//定义一行数据为字符串
	getline(_csvInput, _Oneline);	//读取一行数据

	istringstream _Readstr(_Oneline);	//定义字符串流对象

	string _partOfstr;	//定义一行数据中的各个字串
	getline(_Readstr, _partOfstr, ',');//将一行数据按'，'分隔读出
	user[i].num = _partOfstr;
	getline(_Readstr, _partOfstr, ',');
	user[i].level = _partOfstr;
	getline(_Readstr, _partOfstr, ',');
	user[i].password = _partOfstr;
	getline(_Readstr, _partOfstr, ',');
	user[i].temp.member_name = _partOfstr;
	getline(_Readstr, _partOfstr, ',');
	user[i].temp.member_number = _partOfstr;
	getline(_Readstr, _partOfstr, ',');
	user[i].temp.age = _partOfstr;
	getline(_Readstr, _partOfstr, ',');
	user[i].temp.sex = _partOfstr;
}
void User::read()
{
	ifstream ifile;
	ifile.open("user.csv", ios_base::in);

	scount = 0;
	int i = 0;
	while (ifile)
	{
		csv_read(i, ifile);
		scount++;
		i++;
	}
	scount--;
	ifile.close();
}
//注册
void User::Registers()
{
	read();//读取已储存用户数据
	string ph;//账号
	string level;//等级
	string pw1;//密码1
	string pw2;//密码2
	for (int i = scount; i < SIZE; i++)//从scount之后开始记录
	{
	here:

		cout << "\t\t\t【系统提示】请输入账号：";
		cin >> ph;
		//判断新输入的用户信息是否已存在（如果已存在则重新输入）
		for (int i = 0; i < scount; i++)
		{
			if (ph == user[i].num)
			{
				cout << "\t\t\t【系统提示】用户已存在！" << endl;
				goto here;
			}
		}
		user[i].num = ph;
		system("cls");

		cout << "\t\t\t【系统提示】请输入您的等级：" << endl;
		cout << endl;
		cout << "\t\t\t┃       1.主管     2.前台人员     3.仓库管理员     ┃\t\n";
		cout << endl;
		cout << "\t\t\t【系统提示】请输入你的选择：";
		cin >> level;
		user[i].level = level;
		system("cls");



		int chose = -1;
		cout << endl;
		cout << "\t\t\t┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
		cout << "\t\t\t┃       1.显示密码     2.隐藏密码      ┃\t\n";
		cout << "\t\t\t┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
		cout << "\t\t\t【系统提示】请输入你的选择：";
		cin >> chose;
		if (chose > 2 || chose < -1)
		{
			cout << "\t\t\t【系统提示】输入格式错误，请重新输入：";
			cin >> chose;
		}

		string pword;
		char ch, passwords0[20];
		int x = 0;
		string pword1;
		char ch1, passwords1[20];
		int x1 = 0;
		switch (chose)
		{
		case 1:
			cout << "\t\t\t【系统提示】请输入密码：";
			cin >> pword;
			user[i].password = pword;
			cout << "\t\t\t【系统提示】请再次密码：";
			cin >> pword1;
			if (pword1 != user[i].password)
			{
				cout << "\t\t\t【系统提示】密码不一致！" << endl;
				goto here;
			}
			else
			{
				scount++;//已注册用户加1
				cout << "\t\t\t【系统提示】注册成功！" << endl;
				//提示输入更多数据
				cout << "\t\t\t【系统提示】请输入姓名：";
				cin >> user[i].temp.member_name;
				cout << "\t\t\t【系统提示】请输入编号：";
				cin >> user[i].temp.member_number;
				cout << "\t\t\t【系统提示】请输入年龄：";
				cin >> user[i].temp.age;
				cout << "\t\t\t【系统提示】请输入性别：";
				cin >> user[i].temp.sex;
				save();//保存用户数据
			}
			break;
		case 2:
			cout << "\t\t\t【系统提示】请输入密码：";
			while ((ch = _getch()) != '\r' && x <= 20)
			{
				if (ch == '\b')
				{
					if (x > 0)
					{
						x--;
						cout << "\b \b";//密码支持退格的实现
					}
					else
						putchar(7);
				}
				else
				{
					passwords0[x++] = ch;
					printf("*");
				}
			}
			passwords0[x] = '\0';
			cout << endl;
			user[i].password = passwords0;

			cout << "\t\t\t【系统提示】请再次密码：";
			while ((ch1 = _getch()) != '\r' && x1 <= 20)
			{
				if (ch1 == '\b')
				{
					if (x1 > 0)
					{
						x1--;
						cout << "\b \b";//密码支持退格的实现
					}
					else
						putchar(7);
				}
				else
				{
					passwords1[x1++] = ch1;
					printf("*");
				}
			}
			passwords1[x1] = '\0';
			cout << endl;
			//比较两次输入的密码是否统一（如果不统一则重新输入）
			if (passwords1 != user[i].password)
			{
				cout << "\t\t\t【系统提示】密码不一致！" << endl;
				goto here;
			}
			else
			{
				scount++;//已注册用户加1
				cout << "\t\t\t【系统提示】注册成功！" << endl;
				//提示输入更多数据
				cout << "\t\t\t【系统提示】请输入姓名：";
				cin >> user[i].temp.member_name;
				cout << "\t\t\t【系统提示】请输入编号：";
				cin >> user[i].temp.member_number;
				cout << "\t\t\t【系统提示】请输入年龄：";
				cin >> user[i].temp.age;
				cout << "\t\t\t【系统提示】请输入性别：";
				cin >> user[i].temp.sex;
				save();//保存用户数据
			}
			break;
		}
		char choice;
		cout << "\t\t\t【系统提示】是否继续注册(Y/N)？ :";
		while (1)
		{
			cin >> choice;
			if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N')
				break;
			else
				cout << "\t\t\t【系统提示】输入格式错误，请重新输入： ";
		}
		if (choice == 'n' || choice == 'N')
		{
			break;
		}
	}
}
//登录
void User::Login()
{
	read();//读取已储存用户数据
	string ph;//账号
	string pw;//密码
	int time = 0;//统计比较次数
	cout << "\t\t\t【系统提示】请输入账号：";
	cin >> ph;
	int chose = -1;
	cout << endl;
	cout << "\t\t\t┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
	cout << "\t\t\t┃       1.显示密码     2.隐藏密码      ┃\t\n";
	cout << "\t\t\t┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
	cout << "\t\t\t【系统提示】请输入你的选择：";
	cin >> chose;
	if (chose > 2 || chose < -1)
	{
		cout << "\t\t\t【系统提示】输入格式错误，请重新输入：";
		cin >> chose;
	}
	string pword;
	char ch, passwords0[20];
	int x = 0;
	int flag = 0;
	switch (chose)
	{
	case 1:
		while (!flag)
		{
			cout << "\t\t\t【系统提示】请输入密码：";
			cin >> pword;
			flag = 0;
			for (int i = 0; i < scount; i++)
			{
				//cout << user[i].num << '\t' << user[i].password << endl;
				if (ph == user[i].num && pword == user[i].password)
				{
					flag = 1;
					time++;
					cout << "\t\t\t【系统提示】登录成功！" << endl;
					//在这里判断一下，创建相应对象，再读剩下的信息，并赋值，这样数组里每一个类的信息都全了。
					//然后就能在登录的时候查找到相应位置，并且能够调用各自的成员函数
					if (user[i].level == "1")
					{

						func_manager(i);
					}
					else if (user[i].level == "2")
					{
						func_counter(i);
					}
					else if (user[i].level == "3")
					{
						
						func_warehouser(i);
					}
				}
			}
			if (!flag)	
				cout << " \t\t\t【系统提示】密码错误，请重新输入" << endl;

		}
		break;
	case 2:
		cout << "\t\t\t【系统提示】请输入密码：";
		while ((ch = _getch()) != '\r' && x <= 20)
		{
			if (ch == '\b')
			{
				if (x > 0)
				{
					x--;
					cout << "\b \b";//密码支持退格的实现
				}
				else
					putchar(7);
			}
			else
			{
				passwords0[x++] = ch;
				printf("*");
			}
		}
		passwords0[x] = '\0';
		cout << endl;
		//依次比较已储存信息，比较是否匹配（如不匹配则提示错误）
		for (int i = 0; i < scount; i++)
		{
			if (ph == user[i].num && passwords0 == user[i].password)
			{
				time++;
				cout << "\t\t\t【系统提示】登录成功！" << endl;
				if (user[i].level == "1")
				{
					manager c;
					temp = c;
					func_manager(i);
				}
				else if (user[i].level == "2")
				{
					counter c;
					temp = c;
					func_counter(i);
				}
				else if (user[i].level == "3")
				{
					warehouser w;
					temp = w;
					func_warehouser(i);
				}
			}
		}
		break;
	}
}
