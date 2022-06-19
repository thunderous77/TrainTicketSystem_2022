#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions_2.hpp"

#include <climits>
#include <cstddef>
#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
namespace sjtu 
{
/**
 * a data container like std::Vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class Vector 
{
private:
	T *Array;
	int Maxlen,len;
public:
	/**
	 * TODO
	 * a type for actions of the elements of a Vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator 
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the 
	// iterator points to. 
	// STL algorithms and containers may use these type_traits (e.g. the following 
	// typedef) to work properly. In particular, without the following code, 
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		T *p;
		Vector<T> *Belong;

		friend class const_iterator;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator(){}
		iterator(T *q,Vector<T> *tmp){p=q,Belong=tmp;}
		iterator(const iterator &other){p=other.p,Belong=other.Belong;}
		T* Getp(){return p;}
		iterator operator+(const int &n) const 
		{
			//TODO
			return iterator(p+n,Belong);
		}
		iterator operator-(const int &n) const 
		{
			//TODO
			return iterator(p-n,Belong);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invalid_iterator.
		int operator-(const iterator &rhs) const 
		{
			if(Belong!=rhs.Belong)throw invalid_iterator();
			return p-rhs.p;
			//TODO
		}
		iterator& operator+=(const int &n) 
		{
			p+=n;
			return *this;
			//TODO
		}
		iterator& operator-=(const int &n) 
		{
			p-=n;
			return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp=*this;
			p++;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			++p;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp=*this;
			p--;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			--p;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{return *p;}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {return p==rhs.p;}
		bool operator==(const const_iterator &rhs) const {return p==rhs.p;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return *this==rhs?0:1;}
		bool operator!=(const const_iterator &rhs) const {return *this==rhs?0:1;}
	};

	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator 
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
	
	private:
		/*TODO*/
		const T *p;
		Vector<T> *Belong;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		const_iterator(){}
		const_iterator(const T *q,Vector<T> *tmp){p=q,Belong=tmp;}
		const_iterator(const iterator &other){p=other.p,Belong=other.Belong;}
		const_iterator(const const_iterator &other){p=other.p,Belong=other.Belong;}
		const T* Getp(){return p;}
		const_iterator operator+(const int &n) const 
		{
			//TODO
			return const_iterator(p+n,Belong);
		}
		const_iterator operator-(const int &n) const 
		{
			//TODO
			return const_iterator(p-n,Belong);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invalid_iterator.
		int operator-(const const_iterator &rhs) const 
		{
			if(Belong!=rhs.Belong)throw invalid_iterator();
			return p-rhs.p;
			//TODO
		}
		const_iterator& operator+=(const int &n) 
		{
			p+=n;
			return *this;
			//TODO
		}
		const_iterator& operator-=(const int &n) 
		{
			p-=n;
			return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
			const_iterator tmp=*this;
			p++;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {
			++p;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			const_iterator tmp=*this;
			p--;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() {
			--p;
			return *this;
		}
		/**
		 * TODO *it
		 */
		const T& operator*() const{return *p;}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {return p==rhs.p;}
		bool operator==(const const_iterator &rhs) const {return p==rhs.p;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return *this==rhs?0:1;}
		bool operator!=(const const_iterator &rhs) const {return *this==rhs?0:1;}
	};
private:
	iterator Start,Finish;
public:
	#define Get(a,b) new ((a)) T((b))
	/**
	 * TODO Constructs
	 * Atleast two: default constructor, copy constructor
	 */
	Vector() {
		Maxlen=10,len=0;
		Array=(T*)malloc(Maxlen*sizeof(T));
		Start=iterator(Array,this),Finish=iterator(Array,this);
	}
	Vector(const Vector &other) {
		Maxlen=other.Maxlen,len=other.len;
		Array=(T*)malloc(Maxlen*sizeof(T));
		for(int i=0;i<len;i++) Get(&Array[i],other.Array[i]);//调用拷贝构造函数
		Start=iterator(Array,this),Finish=iterator(Array+len,this);
	}
	/**
	 * TODO Destructor
	 */
	~Vector() {
		for(int i=0;i<len;i++)Array[i].~T();
		free(Array);
	}
	/**
	 * TODO Assignment operator
	 */
	Vector &operator=(const Vector &other) {
		if(this==&other)return *this;
		for(int i=0;i<len;i++)Array[i].~T();
		free(Array);
		Maxlen=other.Maxlen,len=other.len;
		Array=(T*)malloc(Maxlen*sizeof(T));
		for(int i=0;i<len;i++) Get(&Array[i],other.Array[i]);//调用拷贝构造函数
		Start=iterator(Array,this),Finish=iterator(Array+len,this);
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if(pos<0||pos>=len)throw index_out_of_bound();
		return Array[pos];
	}
	const T & at(const size_t &pos) const {
		if(pos<0||pos>=len)throw index_out_of_bound();
		return Array[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if(pos<0||pos>=len)throw index_out_of_bound();
		return Array[pos];
	}
	const T & operator[](const size_t &pos) const {
		if(pos<0||pos>=len)throw index_out_of_bound();
		return Array[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if(len==0)throw container_is_empty();
		return Array[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if(len==0)throw container_is_empty();
		return Array[len-1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {return Start;}
	const_iterator cbegin() const {return Start;}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {return Finish;	}
	const_iterator cend() const {return Finish;}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return len==0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return len;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for(int i=0;i<len;i++)Array[i].~T();
		free(Array);
		Maxlen=10,len=0;
		Array=(T*)malloc(Maxlen*sizeof(T));
		Start=iterator(Array,this),Finish=iterator(Array,this);
	}
	
	void Multiple(){
		T *tmp=(T*)malloc(len*sizeof(T));
		for(int i=0;i<len;i++) Get(&tmp[i],Array[i]);
		for(int i=0;i<len;i++)Array[i].~T();
		free(Array);
		Maxlen<<=1;
		Array=(T*)malloc(Maxlen*sizeof(T));
		for(int i=0;i<len;i++) Get(&Array[i],tmp[i]);
		for(int i=0;i<len;i++)tmp[i].~T();
		free(tmp);
		Start=iterator(Array,this),Finish=iterator(Array+len,this);
	}
	
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		int ind=pos-begin();
		if(len==Maxlen)Multiple();
		pos=begin()+ind;
		for(iterator it=end();it!=pos;it--)Get(it.Getp(),*(it-1));
		Get(pos.Getp(),value);
		len++,Finish++;
		return pos;
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if(ind>len)throw index_out_of_bound();
		if(len==Maxlen)Multiple();
		for(int i=len-1;i>=ind;i--)Get(&Array[i+1],Array[i]);
		Get(&Array[ind],value);
		len++,Finish++;
		return begin()+ind;
	}
	
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		for(iterator it=pos;it!=end()-1;it++)Get(it.Getp(),*(it+1));
		len--,Finish--,Array[len].~T();
		return pos;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if(ind>=len)throw index_out_of_bound();
		for(int i=ind;i<len-1;i++)Get(&Array[i],Array[i+1]);
		len--,Finish--,Array[len].~T();
		return begin()+ind;
	}
	
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if(len==Maxlen)Multiple();
		Get(&Array[len],value);
		len++,Finish++;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if(len==0)throw container_is_empty();
		len--,Finish--,Array[len].~T();
	}
};


}

#endif
