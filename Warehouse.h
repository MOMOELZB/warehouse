#pragma once
#include<iostream>
#include<string>
#include<ctime>
using namespace std;
class warehouse
{
public:
	int warehouse_number=0;
	string warehouse_name;
	int shelf_number;
	warehouse(int warehouse_number, const string& warehouse_name, int shelf_number)
		: warehouse_number(warehouse_number), warehouse_name(warehouse_name), shelf_number(shelf_number)
	{
		cout << "create goods successfully!" << endl;
	};
	warehouse() = default;
	void getwarehousemessage()
	{
		cout << "warehouse number: " << warehouse_number << " warehouse name :" << warehouse_name << endl;
	}
};
class goods
{
public:
	string goods_number=" ";
	string supplier_name=" ";
	string goods_description = "";
	string type = "";
	int measurement = 0;//毫升：1，升：2
	double retall_price=0;
	double sale_price = 0;


	goods(const goods& temp)
	{
		this->supplier_name  = temp.supplier_name ;
		this->goods_number = temp.goods_number;
		this->goods_description = temp.goods_description;
		this->retall_price = temp.retall_price;
		this->sale_price = temp.sale_price;
		this->type = temp.type;
		this->measurement = temp.measurement;
	}
	goods()
	{
		 goods_number = " ";
		 supplier_name = " ";
		 goods_description = "";
		 type = "";
		measurement = 0;//毫升：1，升：2
		 retall_price = 0;
	 sale_price = 0;
	}

	goods(const string& goods_number, const string& goods_name, const string& goods_description, const string& type, int measurement, double retall_price, double sale_price)
		: goods_number(goods_number), supplier_name(goods_name), goods_description(goods_description), type(type), measurement(measurement), retall_price(retall_price), sale_price(sale_price)
	{
	}
};
class goods_shelf:public warehouse
{
	

public:
int location=0;
	goods_shelf(int & location, int warehouse_number, const string& warehouse_name,int shelf_number)
		: location(location), warehouse(warehouse_number, warehouse_name, shelf_number)
	{
		
	}
	goods_shelf(int & location, warehouse & temp) : location(location), warehouse(temp)
	{    
		
	}
	goods_shelf(const goods_shelf& location):warehouse(location)
	{
		this->location = location.location;
	}
	int getlocation() {
		return location;
	}
	goods_shelf() = default;
};
string rtime() {  //以字符串形式返回当前系统时间
	// 基于当前系统的当前日期/时间
	time_t now = time(0);
	tm* ltm = localtime(&now);
	ltm->tm_year += 1900;
	ltm->tm_mon += 1;
	return to_string(ltm->tm_year) +"/" + to_string(ltm->tm_mon)+"/" + to_string(ltm->tm_mday)+"/";
}
string num_time()
{
	time_t now = time(0);
	tm* ltm = localtime(&now);
	ltm->tm_year += 1900;
	ltm->tm_mon += 1;
	return to_string(ltm->tm_year)  + to_string(ltm->tm_mon) + to_string(ltm->tm_mday)  + to_string(ltm->tm_hour)  + to_string(ltm->tm_min) + to_string(ltm->tm_sec);
}
class ware_goods :public goods, public goods_shelf
{
	
    
public:
    double amount=0;
	string goods_time="";
	int  early=-1;
	int  next=-1;
	int location_num = -1;

	ware_goods(const goods& type, const goods_shelf& location, double amount)
		: goods(type), amount(amount),goods_shelf(location)
	{
		goods_time = rtime();
	}
	ware_goods()
	{
		
	}
	ware_goods(const goods_shelf& location, double amount=0)
		: amount(amount), goods_shelf(location)
	{
		goods_time = rtime();
	}
	void goodschange(goods temp, double amount)
	{
		this->supplier_name = temp.supplier_name;
		this->goods_number = temp.goods_number;
		this->goods_description = temp.goods_description;
		this->retall_price = temp.retall_price;
		this->sale_price = temp.sale_price;
		this->type = temp.type;
		this->measurement = temp.measurement;
		//this->goods_time = rtime();
		this->amount = amount;
	}
	void goods_change(goods temp, double amount1)
	{
		supplier_name = temp.supplier_name;
		goods_number = temp.goods_number;
		goods_description = temp.goods_description;
		retall_price = temp.retall_price;
		sale_price = temp.sale_price;
		type = temp.type;
		measurement = temp.measurement;
		amount = amount1;
	}
	void changegoodstime(string time)
	{
		this->goods_time = time;
	}
	void getwaregoods()
	{
		cout << this->goods_time<<" \t  "<<this->goods_number << " \t\t " << this->supplier_name << " \t\t " << this->goods_description << "\t\t" << this->amount << "\t\t " << this->warehouse_name << " \t" <<location_num<<" "<<next << endl;
	}
	
};
