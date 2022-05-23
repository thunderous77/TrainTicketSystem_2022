#ifndef MEMORYRIVER_HPP
#define MEMORYRIVER_HPP

#include <fstream>
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

//T为存储类型，info_len为开头int类型个数(info一开始都设为0),info中的第一位存的是数据占据的最大位置(即要求info_len>=1)
template<class T,int info_len = 1>
class MemoryRiver {
private:
    /* your code here */
    fstream file,file2,file3;
    string file_name;
	string file_name2;// _recycling
	string file_name3;// _rollback    (type,pos,data,type2,pos2,data2)  (int,int,T,int,int,int) 
	// type: delete -1;add 1;update 0
    int sizeofT = sizeof(T);
	int sizeofT3 = sizeofT+sizeof(int)*5;
	int num,num2,num3;
	bool IsRollback;

	void read2_las(int &t){
		int index=num2*sizeof(int);
		file2.open(file_name2);
		file2.seekp(index);
		file2.read(reinterpret_cast<char*>(&t),sizeof(int));
		file2.close();
	}
	int write2(int &t) {
		int index=(num2+1)*sizeof(int);
		num2++;
		file2.open(file_name2);
		file2.seekp(index);
		file2.write(reinterpret_cast<char*>(&t),sizeof(int));
		file2.seekp(0);
		file2.write(reinterpret_cast<char*>(&num2),sizeof(int));
		file2.close();
		return index;
    }
	void initialise2(const string FN,bool ReMake=0){
		file_name2=FN;
		file2.open(file_name2);
		if(!file2||ReMake==1){
			file2.close();
			// std::cout<<"ReMake"<<" "<<file_name2<<" "<<info_len<<std::endl;
			file2.open(file_name2,std::ios::out);//若文件不存在自动创建
			num2=0;
			file2.write(reinterpret_cast<char*>(&num2),sizeof(int));
			file2.close();
		}
		else {
			file2.seekg(0);
			file2.read(reinterpret_cast<char*>(&num2),sizeof(int));
			file2.close();
		}
	}
	void initialise3(const string FN,bool ReMake=0){
		file_name3=FN;
		file3.open(file_name3);
		if(!file3||ReMake==1){
			file3.close();
			// std::cout<<"ReMake"<<" "<<file_name2<<" "<<info_len<<std::endl;
			file3.open(file_name3,std::ios::out);//若文件不存在自动创建
			num3=0;
			file3.write(reinterpret_cast<char*>(&num3),sizeof(int));
			file3.close();
		}
		else {
			file3.seekg(0);
			file3.read(reinterpret_cast<char*>(&num3),sizeof(int));
			file3.close();
		}
	}
	void file3_update(int type,int pos,T data,int type2,int pos2,int data2){
		file3.open(file_name3);
		int index=sizeof(int)+num3*sizeofT3;
		// cout<<"index: "<<index<<endl;
		// cout<<"file3_update "<<type<<" "<<pos<<" "<<type2<<" "<<pos2<<" "<<data2<<endl;
		file3.seekp(index);
		file3.write(reinterpret_cast<char*>(&type),sizeof(int));
		file3.write(reinterpret_cast<char*>(&pos),sizeof(int));
		file3.write(reinterpret_cast<char*>(&data),sizeofT);
		file3.write(reinterpret_cast<char*>(&type2),sizeof(int));
		file3.write(reinterpret_cast<char*>(&pos2),sizeof(int));
		file3.write(reinterpret_cast<char*>(&data2),sizeof(int));

		num3++;
		file3.seekp(0);
		file3.write(reinterpret_cast<char*>(&num3),sizeof(int));
		file3.close();
	}
public:
	void initialise(const string FN,bool ReMake=0) {
		file_name = FN;
		file.open(file_name);
		if(!file||ReMake==1){
			file.close();
			// std::cout<<"ReMake"<<" "<<file_name<<" "<<info_len<<std::endl;
			file.open(file_name,std::ios::out);//若文件不存在自动创建，且会清空文件
			num = 0;
			for (int i = 0; i < info_len; ++i)file.write(reinterpret_cast<char *>(&num), sizeof(int));
			file.close();
		}
		else {
			file.seekp(0);
			file.read(reinterpret_cast<char*>(&num),sizeof(int));
			file.close();
		}
		initialise2(FN+"_memory_recycling",ReMake);
		if(IsRollback)initialise3(FN+"_inside_rollback",ReMake);
    }
    MemoryRiver(const string FN="",bool _IsRollback=0){
		_IsRollback=0;//关闭rollback指令
		IsRollback=_IsRollback;
		if(FN!="")initialise(FN);
	}

    //读出第n个int的值赋给tmp，1_base
    void read_info(int &tmp, int n) {
        if (n > info_len) return;
		file.open(file_name);
		file.seekp((n-1)*sizeof(int));
		file.read(reinterpret_cast<char*>(&tmp),sizeof(int));
		file.close();
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
		file.open(file_name);
		file.seekp((n-1)*sizeof(int));
		file.write(reinterpret_cast<char*>(&tmp),sizeof(int));
		file.close();
    }

	
    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(T &t) {
		file.open(file_name);
		int index;
		if(num2){
			read2_las(index);
			
			int pos2=num2*sizeof(int),data2;
			file2.seekp(pos2);
			file2.read(reinterpret_cast<char*>(&data2),sizeof(int));

			num2--;
			file2.open(file_name2);
			file2.seekp(0);
			file2.write(reinterpret_cast<char*>(&num2),sizeof(int));
			file2.close();

			if(IsRollback)file3_update(1,index,T(),-1,pos2,data2);
		}
		else {
			index=info_len*sizeof(int)+num*sizeofT;

			if(IsRollback)file3_update(1,index,T(),0,-1,0);
		}
		num++;
		file.seekp(0);
		file.write(reinterpret_cast<char*>(&num),sizeof(int));

		file.seekp(index);
		file.write(reinterpret_cast<char*>(&t),sizeofT);
		file.close();
		return index;
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
		file.open(file_name);
		T data;
		file.seekp(index);
		file.read(reinterpret_cast<char*>(&data),sizeofT);

		file.seekp(index);
		file.write(reinterpret_cast<char*>(&t),sizeofT);
		file.close();

		if(IsRollback)file3_update(0,index,data,0,-1,0);
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
		file.open(file_name);
		file.seekp(index);
		file.read(reinterpret_cast<char*>(&t),sizeofT);
		file.close();
        /* your code here */
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index,bool type=0) {//type=1时不回收空间,保证删除的是最后一个(同时保证IsRollback=0)
		if(!type){
			T data;
			file.open(file_name);
			file.seekg(index);
			file.read(reinterpret_cast<char*>(&data),sizeofT);
			num--;
			file.seekp(0);
			file.write(reinterpret_cast<char*>(&num),sizeof(int));
			file.close();
			int pos2=(num2+1)*sizeof(int);

			write2(index);
			
			if(IsRollback)file3_update(-1,index,data,1,pos2,0);
		}
		else {
			num--;
			file.open(file_name);
			file.seekp(0);
			file.write(reinterpret_cast<char*>(&num),sizeof(int));
			file.close();
		}
    }
	
	//清空文件
	void clean(){
		initialise(file_name,1);
	}

	//找到最后一个数据的位置
	int Maxpos(){
		if(!num)return -1;
		return info_len*sizeof(int)+(num-1)*sizeofT;
	}

	void rollback(){//回滚一次操作(write/delete/update)
		if(!num3)return;
		file3.open(file_name3);
		int index=sizeof(int)+(num3-1)*sizeofT3;
		// (type,pos,data,type2,pos2,data2)  (int,int,T,int,int,int) 
		// type: delete -1;add 1;update 0
		// cout<<"num3,index: "<<num3<<" "<<index<<endl;
		int type,type2,pos,pos2,data2;
		T data;
		file3.seekg(index);
		file3.read(reinterpret_cast<char*>(&type),sizeof(int));
		file3.read(reinterpret_cast<char*>(&pos),sizeof(int));
		file3.read(reinterpret_cast<char*>(&data),sizeofT);
		file3.read(reinterpret_cast<char*>(&type2),sizeof(int));
		file3.read(reinterpret_cast<char*>(&pos2),sizeof(int));
		file3.read(reinterpret_cast<char*>(&data2),sizeof(int));

		// cout<<"@@@ "<<type<<" "<<pos<<" "<<type2<<" "<<pos2<<endl;

		file.open(file_name);
		num+=(-type);
		file.seekp(0);
		file.write(reinterpret_cast<char*>(&num),sizeof(int));
		if(pos!=-1){
			file.seekp(pos);
			file.write(reinterpret_cast<char*>(&data),sizeofT);
		}
		file.close();
		
		file2.open(file_name2);
		num2+=(-type2);
		file2.seekp(0);
		file2.write(reinterpret_cast<char*>(&num2),sizeof(int));
		if(pos2!=-1){
			file2.seekp(pos2);
			file2.write(reinterpret_cast<char*>(&data2),sizeof(int));
		}
		file2.close();
		
		num3--;
		file3.seekp(0);
		file3.write(reinterpret_cast<char*>(&num3),sizeof(int));
		file3.close();
	}
};

#endif