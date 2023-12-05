#pragma once
#include<fstream>
#include <iostream>
#include"warehouse.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;
int delivery_index(goods temp)
{
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
        if (temp.goods_number == " ") {
            if (index[i].goods_number == " ") {
                int result = index[i].location_num;
                fclose(fp);
                return result;
            }
        }
        else {
            if (index[i].goods_number == temp.goods_number)
            {
                fclose(fp);
                  return index[i].location_num;
            }
              
            if (index[i].goods_number == " ")
            {
                fclose(fp);
                return -1;
            }
        }
    }

}
int Storage_index(goods temp, double amount)
{
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
        if (i == 199)
                return -1;
        if (temp.goods_number == " ") {
            if (index[i].goods_number == " ") {
                int result = index[i].location_num;
                index[i] = index[index[index[i].location_num].next];
                fwrite(index, sizeof(ware_goods), filenum, fp);
                fclose(fp);
                return result;
            }
            
        }
        else {
            if (index[i].goods_number == temp.goods_number)
                return index[i].location_num;
            if (index[i].goods_number == " ")
            {
                cout << temp.supplier_name;
                cout << i;
                index[i].goodschange(goods(temp), amount);
                index[i + 1] = index[index[index[i].location_num].next];
                fwrite(index, sizeof(ware_goods), filenum, fp);
                fclose(fp);
                return index[i].location_num;
            }
        }
    }

}
int index_search(goods temp)
{
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
    for (int i = 0; i < 100; i++)
        if (index[i].goods_number == temp.goods_number)
        {
            fclose(fp);
            return i;
        }
    fclose(fp);
    return -1;
}
void setwarehouse()
{
    string warehouse_name;
    int warehouse_number;
    int shelf_number;
    cout << "输入仓库编号，名称以及货架数" << endl;
    cin >> warehouse_number >> warehouse_name >> shelf_number;
    warehouse empty_warehouse(warehouse_number, warehouse_name, shelf_number);
    goods_shelf* empty_shelf = new goods_shelf[shelf_number];
    ware_goods* empty_goods = new ware_goods[shelf_number];
    FILE* fp;
    if (!(fp = fopen("warehouse.dat", "rb+")))
        cout << "create warehouse fail" << endl;
    fseek(fp, 0, SEEK_END);
    int sizelong = ftell(fp);
    int filenum = sizelong / sizeof(ware_goods);
    rewind(fp);
    for (int i = 0; i < shelf_number; i++)
    {
        empty_shelf[i] = goods_shelf(i, empty_warehouse);
        empty_goods[i] = ware_goods(empty_shelf[i]);
        empty_goods[i].location_num = filenum + i;
    }
    for (int i = 0; i < shelf_number; i++)
    {
        if (i != shelf_number - 1)
            empty_goods[i].next = empty_goods[i + 1].location_num;
        if (i != 0)
            empty_goods[i].early = empty_goods[i - 1].location_num;
    }
    int empty_location = delivery_index(goods());
    ware_goods* index = new ware_goods[filenum];
    fread(index, sizeof(ware_goods), filenum, fp);
    while (index[empty_location].next != -1)
        empty_location = index[empty_location].next;
    empty_goods[0].early = empty_location;
    index[empty_location].next = empty_goods[0].location_num;
    rewind(fp);
    fwrite(index, sizeof(ware_goods), filenum, fp);
    fwrite(empty_goods, sizeof(ware_goods), shelf_number, fp);
    fclose(fp);
}
void setindex()
{
    FILE* fp;
    if (!(fp = fopen("warehouse.dat", "wb+")))
        cout << "create warehouse fail" << endl;
    ware_goods* index = new ware_goods[200];
    fwrite(index, sizeof(ware_goods), 200, fp);
    fclose(fp);
}
bool Storage(goods temp, double amount)
{
    FILE* fp;
    if (!(fp = fopen("warehouse.dat", "rb+")))
        cout << "fail_Storage" << endl;
    fseek(fp, 0, SEEK_END);
    long int  sizelong = ftell(fp);
    int filenum = sizelong / sizeof(ware_goods);
    rewind(fp);
 
    int index_goods = Storage_index(temp, amount);
    ware_goods* total_goods = new ware_goods[filenum];
    fread(total_goods, sizeof(ware_goods), filenum, fp);
    rewind(fp);
    if (total_goods[index_goods].goods_number == " ")
    {
        total_goods[index_goods].goods_change(temp, amount);
        total_goods[index_goods].next = -1;
        rewind(fp);
        fwrite(total_goods, sizeof(ware_goods), filenum, fp);
        fclose(fp);
        return true;
    }
    int empty_goods = Storage_index(goods(), 1);
    fread(total_goods, sizeof(ware_goods), filenum, fp);
    rewind(fp);
    if (empty_goods == -1)
        return false;
    //判断是否有空位
    total_goods[empty_goods].goodschange(temp, amount);
    while (total_goods[index_goods].next != -1)
        index_goods = total_goods[index_goods].next;
    total_goods[index_goods].next = total_goods[empty_goods].location_num;
    total_goods[empty_goods].early = index_goods;
    total_goods[empty_goods].next = -1;
    rewind(fp);
    fwrite(total_goods, sizeof(ware_goods), filenum, fp);
    fclose(fp);
    return true;
}
bool delivery(goods temp, double amount)
{

    int goods_index = delivery_index(temp);
    if (goods_index == -1)
    {
        cout << "fail index" << endl;
        return false;
    }

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
    int empty_goods = delivery_index(goods());
    while (index[empty_goods].next != -1)
        empty_goods = index[empty_goods].next;
    int goods_index_location = index_search(temp);
    while (amount > 0) {
        amount -= index[goods_index].amount;
        if (amount < 0)
        {
            index[goods_index].amount = -amount;
            fwrite(index, sizeof(ware_goods), filenum, fp);
            fclose(fp);
            cout << "p1";
            return true;
        }
        index[goods_index].goodschange(goods(), 0);
        index[goods_index].early = empty_goods;
        int temp_goods = index[goods_index].next;
        if (temp_goods != -1)
            index[goods_index_location] = index[temp_goods];
        index[goods_index].next = -1;
        index[empty_goods].next = goods_index;
        empty_goods = goods_index;
        goods_index = temp_goods;
        if (temp_goods == -1 && amount > 0)
        {
            cout << "not enough" << endl;
            fclose(fp);
            return false;
        }

        if (temp_goods != -1 && amount == 0)
        {
            int goods_index_ = index_search(temp);
            index[goods_index_] = index[temp_goods];
            fwrite(index, sizeof(ware_goods), filenum, fp);
            fclose(fp);
            fclose(fp);
            return true;
        }
        if (temp_goods == -1 && amount == 0)
        {
            while (index[goods_index_location].goods_number != " ")
            {
                index[goods_index_location] = index[goods_index_location + 1];

                index[goods_index_location].location_num = index[goods_index_location + 1].location_num;
                goods_index_location++;
            }
            index[goods_index_location + 1].next = -1;
            cout << " enough" << endl;
            fwrite(index, sizeof(ware_goods), filenum, fp);
            fclose(fp);
            cout << "p";
            return true;
        }
    }
}
void getgoods_index()
{
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
    for (int i = 0; i < 200; i++)
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
            cout << index[mid].goods_number << "\t\t" <<index[mid].supplier_name<<"\t\t\t"<<index[mid].goods_description<<"\t\t\t"<< count << endl;
        }
    }
    fclose(fp);
}
void getfile()
{
    FILE* fp;
    if (!(fp = fopen("warehouse.dat", "rb+")))
        cout << "create warehouse fail" << endl;
    fseek(fp, 0, SEEK_END);
    int sizelong = ftell(fp);
    int filenum = sizelong / sizeof(ware_goods);
    rewind(fp);
    ware_goods* index = new ware_goods[filenum];
    fread(index, sizeof(ware_goods), filenum, fp);
    cout << "入库日期 \t|产品编号  \t|供货商  \t|产品描述 \t|数量 \t|仓库 \t|" << endl;
   // for (int i = 0; i < 200; i++)
    {
      

    }
    for (int i = 0; i < filenum; i++)
    {
      //  if (index[i].goods_number != " ")
            index[i].getwaregoods();
    }
    fclose(fp);
};
void setsystem()
{
    setindex();
    string warehouse_name;
    int warehouse_number;
    int shelf_number;
    cout << "输入仓库编号，名称以及货架数" << endl;
    cin >> warehouse_number >> warehouse_name >> shelf_number;
    warehouse empty_warehouse(warehouse_number, warehouse_name, shelf_number);
    goods_shelf* empty_shelf = new goods_shelf[shelf_number];
    ware_goods* empty_goods = new ware_goods[shelf_number];
    FILE* fp;
    if (!(fp = fopen("warehouse.dat", "rb+")))
        cout << "create warehouse fail" << endl;
    fseek(fp, 0, SEEK_END);
    int sizelong = ftell(fp);
    int filenum = sizelong / sizeof(ware_goods);
    rewind(fp);
    for (int i = 0; i < shelf_number; i++)
    {
        empty_shelf[i] = goods_shelf(i, empty_warehouse);
        empty_goods[i] = ware_goods(empty_shelf[i]);
        empty_goods[i].location_num = filenum + i;
    }
    for (int i = 0; i < shelf_number; i++)
    {
        if (i != shelf_number - 1)
            empty_goods[i].next = empty_goods[i + 1].location_num;
        if (i != 0)
            empty_goods[i].early = empty_goods[i - 1].location_num;
    }
    ware_goods* index = new ware_goods[filenum];
    fread(index, sizeof(ware_goods), filenum, fp);
    index[0] = empty_goods[0];
    rewind(fp);
    fwrite(index, sizeof(ware_goods), filenum, fp);
    fwrite(empty_goods, sizeof(ware_goods), shelf_number, fp);
    sizelong = ftell(fp);
    filenum = sizelong / sizeof(ware_goods);
    fclose(fp);
}
void input()
{
    cout << "输入入库商品批数:";
    int n;
    cin >> n;
    string goods_num, suppiler, des, type;
    int mea;
    double retall, sale, amont;
    for (int i = 0; i < n; i++)
    {
        cout << endl << "依次输入商品编号，供货商，描述，种类，容量单位（毫升：1，升：2），进价，售价" << endl;
        cin >> goods_num >> suppiler >> des >> type >> mea >> retall >> sale;

        goods temp(goods_num, suppiler, des, type, mea, retall, sale);
        cout << endl << "数量： ";
        cin >> amont;
        cout << endl;
        while (amont > 0)
        {
            if (mea == 1)
            {
                amont -= 50;
                if (amont > 0)
                    Storage(temp, 50);
            }
            if (mea == 2)
            {
                amont -= 30;
                if (amont > 0)
                    Storage(temp, 30);
            }
            cout << "d";
        }
        if (mea == 1)
            amont += 50;
        if (mea == 2)
            amont += 30;
        cout << amont;
        Storage(temp, amont);

        cout << "d";
    }
}
void output()
{
    cout << "输入出库商品批数:";
    int n;
    cin >> n;
    string goods_num, suppiler, des, type;
    int mea;
    double retall, sale, amont;
    for (int i = 0; i < n; i++)
    {
        cout << endl << "依次输入商品编号，供货商，描述，种类，容量单位（毫升：1，升：2），进价，售价" << endl;
        cin >> goods_num >> suppiler >> des >> type >> mea >> retall >> sale;

        goods temp(goods_num, suppiler, des, type, mea, retall, sale);
        cout << endl << "数量： ";
        //cout << " " << amont << endl;
        cin >> amont;
        cout << endl;
        bool t = delivery(temp, amont);
        if (!t)
            cout << "delivery fail" << endl;
        else
            cout << "delivery successfully" << endl;
    }
}
