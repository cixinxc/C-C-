#include <iostream>
#include <sstream>
#include <string.h>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
using namespace rapidxml;
using namespace std;

int str2int(string str)
{
	stringstream ss;
	ss<<str;
	int num = 0;
	ss>>num;
	return	num;
}

int main()
{
	file<> fdoc("Server.xml");
	std::cout<<fdoc.data()<<std::endl;
	xml_document<>   doc;
	doc.parse<0>(fdoc.data());

    	//! 获取根节点
    	xml_node<>* root = doc.first_node();
    	std::cout<<root->name()<<std::endl;

	xml_node<>* current_node = root;
	string command = "";
	while(1)
	{	
		cout << endl;
		int location = 0;
		xml_node<>* current_node_c = current_node;

		string ip_chosed = "";
		int port_chosed = 0;
		string program = "";

		if(current_node->first_node("计算服务器"))
			current_node_c = current_node->first_node("计算服务器");
		else if(current_node->first_node("program"))
			current_node_c = current_node->first_node("program");
		else
		{
			cout<< "wrong input" << endl;
			current_node = root;
			continue;
		}
			
		cout<< "0: chose parent root" << endl;
		do
		{
			cout<< location+1 << ": " << current_node_c->name() << ":" << current_node_c->first_node()->value() <<endl;
			current_node_c = current_node_c->next_sibling();
			location++;
		}while(current_node_c);
	//	cout<< "location:" << location << endl;
		cout<< "plese input your chose:" << endl;
		cin>>command;
		int getL = str2int(command);

	//	cout<< "num is " << getL <<endl;
		if(0<getL && getL<=location)
		{
			if(current_node->first_node())
			{
				if(current_node->first_node("program"))
				{
					current_node = current_node->first_node("program");
					ip_chosed = current_node->parent()->first_node("ip")->value();
					port_chosed=str2int( current_node->parent()->first_node("port")->value() );
					cout<< "ip is : " << ip_chosed << endl << "port is : " << port_chosed <<endl;
	
					
				}
				else if(current_node->first_node("params"))
				{
					current_node = current_node->first_node("params");
					
				}
				
				else
					current_node = current_node->first_node();
				int chirld_l = 1;
				while(current_node->next_sibling() && chirld_l<getL)
				{
					current_node = current_node->next_sibling();
				}
				program = current_node->first_node()->value();
				cout<< "program is : " << program << endl;
/*
else if(current_node->first_node("params"))
				{
					current_node = current_node->first_node("params");
					
					program = current_node->name();
					cout<< "program is : " << program << endl;
				}
*/
				
			}
		}
		else if(getL==0)
		{
			if(current_node->parent())
			{
				current_node = current_node->parent();
			}
		}
		else
		{
			cout<< "wrong input!" << endl;
		}
	}

    return EXIT_SUCCESS;
}
