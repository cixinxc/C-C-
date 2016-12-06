#include <iostream>
#include <sstream>
#include <string.h>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
using namespace rapidxml;
using namespace std;

bool IsNum(string str) 
{
	for(int i=0;i<str.size();i++) 
	{   
		if ((str.at(i)>'9') || (str.at(i)<'0') )
		{ 
 			return false; 
		} 
	} 
	return true; 
} 

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
    std::cout<<doc.name()<<std::endl;
std::cout<<std::endl;
std::cout<<std::endl;
    //! 获取根节点
    xml_node<>* root = doc.first_node();
    std::cout<<root->name()<<std::endl;
    //! 获取根节点第一个节点
    xml_node<>* node1 = root->first_node();
    std::cout<<node1->name()<<std::endl;
    xml_node<>* node11 = node1->first_node();
    
    std::cout<<node11->name()<<std::endl;
    std::cout<<node11->value()<<std::endl;
    //
std::cout<<"root first node"<<std::endl;
    xml_node<>* node120 = root->first_node();
    xml_node<>* node1120 = node120->first_node()->next_sibling();
    
    std::cout<<node1120->name()<<std::endl;
    std::cout<<node1120->value()<<std::endl;
    //
std::cout<< "root next node  "<<std::endl;
    xml_node<>* node12 = root->first_node()->next_sibling();
    xml_node<>* node112 = node12->first_node()->next_sibling();
    
    std::cout<<node112->name()<<std::endl;
    std::cout<<node112->value()<<std::endl;

xml_node<>* nodeP = node12->parent();
std::cout<<nodeP->name()<<std::endl;
std::cout<<nodeP->value()<<std::endl;

std::cout<< "attribute:"<<std::endl;
xml_node<>* nodePa = node12->first_node("program");
std::cout<<nodePa->name()<<std::endl;
std::cout<<nodePa->value()<<std::endl;
std::cout<< "attribute2:"<<std::endl;
std::cout<<nodePa->first_node()->name()<<std::endl;
cout<<nodePa->first_node()->value()<<endl;

	xml_node<>* current_node = root;
	string command = "";
	while(1)
	{	
		cout << endl;
		int location = 0;
		xml_node<>* current_node_c = current_node;

		if(current_node->first_node("计算服务器"))
			current_node_c = current_node->first_node("计算服务器");
		else if(current_node->first_node("program"))
			current_node_c = current_node->first_node("program");
		else
		{
			cout<< "wrong input" << endl;
			current_node = root;
		//	current_node_c = current_node->first_node();
			continue;
		}
			
		cout<< "0: chose parent root" << endl;
		do
		{
			cout<< location+1 << ": " << current_node_c->name() << ":" << current_node_c->first_node()->value() <<endl;
		//	cout<< location+1 << ": " << current_node_c->name() <<endl;
			current_node_c = current_node_c->next_sibling();
			location++;
		}while(current_node_c);
		cout<< "location:" << location << endl;
		cout<< "plese input your chose:" << endl;
		cin>>command;
		int getL = str2int(command);

		cout<< "num is " << getL <<endl;
		if(0<getL && getL<=location)
		{
			if(current_node->first_node())
			{
				if(current_node->first_node("program"))
					current_node = current_node->first_node("program");
				else if(current_node->first_node("params"))
					current_node = current_node->first_node("params");
				else
					current_node = current_node->first_node();
				int chirld_l = 1;
				while(current_node->next_sibling() && chirld_l<getL)
				{
					current_node = current_node->next_sibling();
				}
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
