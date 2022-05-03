#ifndef BLOCKLISTHPP
#define BLOCKLISTHPP
#include<cstring>
#include "MemoryRiver.hpp"
#include<vector>//最后用手写的vector换掉
using namespace std;
template<class T>class Key_value_database{//块状链表实现
private:
	struct node{
		char Key[65];
		T val;
		node(){fill(Key,Key+65,'\0');}
		node(string _Key,T _val){
			fill(Key,Key+65,'\0');
			for(int i=0;i<(int)_Key.size();i++)Key[i]=_Key[i];
			val=_val;
		}
		node& operator =(const node &other){
			if(this==&other)return *this;
			for(int i=0;i<65;i++)Key[i]=other.Key[i];
			val=other.val;
			return *this;
		}
		bool operator <(const node &other)const{
			for(int i=0;i<64;i++)if(Key[i]!=other.Key[i])return Key[i]<other.Key[i];
			return val<other.val;
		}
		bool operator >(const node &other)const{
			for(int i=0;i<64;i++)if(Key[i]!=other.Key[i])return Key[i]>other.Key[i];
			return val>other.val;
		}
		bool operator ==(const node &other)const{
			for(int i=0;i<64;i++)if(Key[i]!=other.Key[i])return 0;
			if(val!=other.val)return 0;
			return 1;
		}
		bool operator !=(const node &other)const{return !((*this)==other);}
	};
	const static int BlockMaxn=1200;
	const static int MergeSize=BlockMaxn*0.4;
	struct Block_info{//表头文件
		int size=0,pre_id=-1,next_id=-1;
		int info_id=-1,data_id=-1;
		node Fir,Las;
	};
	struct Block_data{//数据
		node data[BlockMaxn];
	};
	MemoryRiver<Block_info,3>Blocks_info;//2 blocknum,3 fir_id
	MemoryRiver<Block_data,1>Blocks_data;
public:
	Key_value_database(const string &Blockname):Blocks_info(Blockname+"_info"),Blocks_data(Blockname+"_data"){}
	void Split(Block_info &Now,Block_data &Now2){
		Block_info tmp;
		Block_data tmp2;
		tmp.info_id=Blocks_info.write(tmp);
		tmp.data_id=Blocks_data.write(tmp2);
		tmp.size=Now.size/2,Now.size-=tmp.size;
		for(int i=0;i<tmp.size;i++)tmp2.data[i]=Now2.data[i+Now.size];
		Now.Fir=Now2.data[0],Now.Las=Now2.data[Now.size-1];
		tmp.Fir=tmp2.data[0],tmp.Las=tmp2.data[tmp.size-1];
		tmp.next_id=Now.next_id,tmp.pre_id=Now.info_id;
		if(Now.next_id!=-1){
			Block_info GG_next;
			Blocks_info.read(GG_next,Now.next_id);
			GG_next.pre_id=tmp.info_id;
			Blocks_info.update(GG_next,GG_next.info_id);
		}
		Now.next_id=tmp.info_id;
		
		Blocks_info.update(Now,Now.info_id);
		Blocks_data.update(Now2,Now.data_id);
		Blocks_info.update(tmp,tmp.info_id);
		Blocks_data.update(tmp2,tmp.data_id);
		int blocknum;
		Blocks_info.read_info(blocknum,2);
		Blocks_info.write_info(blocknum+1,2);
	}
	void Merge(Block_info &Now,Block_data &Now2,Block_info &tmp,Block_data &tmp2){
		for(int i=0;i<tmp.size;i++)Now2.data[i+Now.size]=tmp2.data[i];
		Now.size+=tmp.size;
		Now.Fir=Now2.data[0],Now.Las=Now2.data[Now.size-1];
		Now.next_id=tmp.next_id;
		if(tmp.next_id!=-1){
			Block_info GG_next;
			Blocks_info.read(GG_next,tmp.next_id);
			GG_next.pre_id=Now.info_id;
			Blocks_info.update(GG_next,GG_next.info_id);
		}
		Blocks_info.update(Now,Now.info_id);
		Blocks_data.update(Now2,Now.data_id);
		Blocks_info.Delete(tmp.info_id);
		Blocks_data.Delete(tmp.data_id);
		int blocknum;
		Blocks_info.read_info(blocknum,2);
		Blocks_info.write_info(blocknum-1,2);
	}
	void CheckMerge(Block_info &Now,Block_data &Now2){
		Block_info tmp;
		Block_data tmp2;
		if(Now.pre_id!=-1){
			Blocks_info.read(tmp,Now.pre_id);
			if(Now.size==0||Now.size+tmp.size<MergeSize){
				Blocks_data.read(tmp2,tmp.data_id);
				Merge(tmp,tmp2,Now,Now2);
				return;
			}
		}
		if(Now.next_id!=-1){
			Blocks_info.read(tmp,Now.next_id);
			if(Now.size==0||Now.size+tmp.size<MergeSize){
				Blocks_data.read(tmp2,tmp.data_id);
				Merge(Now,Now2,tmp,tmp2);
				return;
			}
		}
		if(Now.size==0&&Now.pre_id==-1&&Now.next_id==-1){
			Blocks_info.Delete(Now.info_id);
			Blocks_data.Delete(Now.data_id);
			Blocks_info.write_info(0,2);
			Blocks_info.write_info(0,3);
		}
	}

	void insert(const string key,T val){
		node value=node(key,val);
		int blocknum;
		Blocks_info.read_info(blocknum,2);
		if(!blocknum){
			Block_info tmp;
			Block_data tmp2;
			tmp.info_id=Blocks_info.write(tmp);
			tmp.data_id=Blocks_data.write(tmp2);
			tmp2.data[0]=value;
			tmp.size=1;
			tmp.Fir=tmp2.data[0],tmp.Las=tmp2.data[0];
			Blocks_info.update(tmp,tmp.info_id);
			Blocks_data.update(tmp2,tmp.data_id);
			Blocks_info.write_info(1,2);
			int blocknum;
			Blocks_info.read_info(blocknum,2);
			Blocks_info.write_info(tmp.info_id,3);
			return;
		}
		else {
			Block_info Now;
			int id;
			Blocks_info.read_info(id,3);
			Blocks_info.read(Now,id);
			while(value>Now.Las&&Now.next_id!=-1){
				id=Now.next_id;
				Blocks_info.read(Now,id);
			}
			Block_data Now2;
			Blocks_data.read(Now2,Now.data_id);
			int size=Now.size;
			int pos=lower_bound(Now2.data,Now2.data+Now.size,value)-Now2.data;
			for(int i=size;i>=pos+1;i--)Now2.data[i]=Now2.data[i-1];
			Now2.data[pos]=value;
			Now.size++;
			Now.Fir=Now2.data[0],Now.Las=Now2.data[Now.size-1];
			Blocks_info.update(Now,Now.info_id);
			Blocks_data.update(Now2,Now.data_id);
			if(Now.size==BlockMaxn)Split(Now,Now2);
		}
	}
	bool Delete(const string key,T val){
		node value=node(key,val);
		int blocknum;
		Blocks_info.read_info(blocknum,2);
		if(!blocknum)return 0;
		else {
			Block_info Now;
			int id;
			Blocks_info.read_info(id,3);
			Blocks_info.read(Now,id);
			while(value>Now.Las&&Now.next_id!=-1){
				id=Now.next_id;
				Blocks_info.read(Now,id);
			}
			Block_data Now2;
			Blocks_data.read(Now2,Now.data_id);
			int size=Now.size;
			int pos=lower_bound(Now2.data,Now2.data+Now.size,value)-Now2.data;
			if(pos<Now.size&&Now2.data[pos]!=value)return 0;
			for(int i=pos;i<=size-2;i++)Now2.data[i]=Now2.data[i+1];
			Now.size--;
			Now.Fir=Now2.data[0],Now.Las=Now2.data[Now.size-1];
			Blocks_info.update(Now,Now.info_id);
			Blocks_data.update(Now2,Now.data_id);
			CheckMerge(Now,Now2);
			return 1;
		}
	}
	vector<int> FindAll(const string key){
		vector<int>ans;
		int blocknum;
		Blocks_info.read_info(blocknum,2);
		if(!blocknum)return ans;
		else {
			Block_info Now;
			int id;
			Blocks_info.read_info(id,3);
			Blocks_info.read(Now,id);
			while(key>Now.Las.Key&&Now.next_id!=-1){
				id=Now.next_id;
				Blocks_info.read(Now,id);
			}
			Block_data Now2;
			bool Is_Find=0;
			while(id!=-1){
				Blocks_info.read(Now,id);
				Blocks_data.read(Now2,Now.data_id);
				int size=Now.size;
				for(int i=0;i<size;i++)if(Now2.data[i].Key==key){ans.push_back(Now2.data[i].val);Is_Find=1;}
				if(Now2.data[Now.size-1].Key!=key)break;
				id=Now.next_id;
			}
		}
		return ans;
	}
	bool Find(const string key){
		int blocknum;
		Blocks_info.read_info(blocknum,2);
		if(!blocknum)return 0;
		else {
			Block_info Now;
			int id;
			Blocks_info.read_info(id,3);
			Blocks_info.read(Now,id);
			while(key>Now.Las.Key&&Now.next_id!=-1){
				id=Now.next_id;
				Blocks_info.read(Now,id);
			}
			Block_data Now2;
			bool Is_Find=0;
			while(id!=-1){
				Blocks_info.read(Now,id);
				Blocks_data.read(Now2,Now.data_id);
				int size=Now.size;
				for(int i=0;i<size;i++)if(Now2.data[i].Key==key)return 1;
				if(Now2.data[Now.size-1].Key!=key)break;
				id=Now.next_id;
			}
		}
		return 0;
	}
	void clean(){
		Blocks_info.clean();
		Blocks_data.clean();
	}
};

#endif