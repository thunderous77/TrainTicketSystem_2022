/**
 * implement a container like std::linked_hashmap
 */
#ifndef SJTU_LINKEDHASHMAP_HPP
#define SJTU_LINKEDHASHMAP_HPP

// only for std::equal_to<T> and std::hash<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions_2.hpp"

namespace sjtu {
    /**
     * In linked_hashmap, iteration ordering is differ from map,
     * which is the order in which keys were inserted into the map.
     * You should maintain a doubly-linked list running through all
     * of its entries to keep the correct iteration order.
     *
     * Note that insertion order is not affected if a key is re-inserted
     * into the map.
     */
    
template<
	class Key,
	class T,
	class Hash = std::hash<Key>, 
	class Equal = std::equal_to<Key>
> class linked_hashmap {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::linked_hashmap as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
	
	struct node{
		value_type *val;
		node *next,*pre;
		node *time_next,*time_pre;
		node():next(nullptr),pre(nullptr),time_next(nullptr),time_pre(nullptr){}
		node(const value_type &_val):next(nullptr),pre(nullptr),time_next(nullptr),time_pre(nullptr){
			val=new value_type(_val);
		}
		node(const node &other){
			val=new value_type(other.val);
			next=nullptr,pre=nullptr,time_next=other.time_next,time_pre=other.time_pre;
		}
		~node(){delete val;}
	};
	int Capacity;
	const double LoadFactor=0.75,LoadFactor2=0.25;
	node **Point;
	node *Fir,*Las;
	int Size;
 
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = linked_hashmap.begin(); --it;
	 *       or it = linked_hashmap.end(); ++end();
	 */
	class const_iterator;
	class iterator {
		friend class linked_hashmap;
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		node *p;
		const linked_hashmap *Belongs;
	public:
		// The following code is written for the C++ type_traits library.
		// Type traits is a C++ feature for describing certain properties of a type.
		// For instance, for an iterator, iterator::value_type is the type that the 
		// iterator points to. 
		// STL algorithms and containers may use these type_traits (e.g. the following 
		// typedef) to work properly. 
		// See these websites for more information:
		// https://en.cppreference.com/w/cpp/header/type_traits
		// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
		// About iterator_category: https://en.cppreference.com/w/cpp/iterator
		// using difference_type = std::ptrdiff_t;
		// using value_type = T;
		// using pointer = T*;
		// using reference = T&;
		// using iterator_category = std::output_iterator_tag;

		iterator() :p(nullptr),Belongs(nullptr){}
		iterator(node *_p,const linked_hashmap *_Belongs):p(_p),Belongs(_Belongs){}
		iterator(const iterator &other) {p=other.p,Belongs=other.Belongs;}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			if(p==nullptr)throw invalid_iterator();
			iterator tmp=*this;
			p=p->time_next;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
			if(p==nullptr)throw invalid_iterator();
			p=p->time_next;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			if(p==Belongs->Fir)throw invalid_iterator();
			iterator tmp=*this;
			p=(p==nullptr?Belongs->Las:p->time_pre);
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
			if(p==Belongs->Fir)throw invalid_iterator();
			p=(p==nullptr?Belongs->Las:p->time_pre);
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {return *(p->val);}
		bool operator==(const iterator &rhs) const {
			if(p==nullptr||rhs.p==nullptr)return p==nullptr&&rhs.p==nullptr&&Belongs==rhs.Belongs;
			else return &(*p)==&(*rhs.p);
		}
		bool operator==(const const_iterator &rhs) const {
			if(p==nullptr||rhs.p==nullptr)return p==nullptr&&rhs.p==nullptr&&Belongs==rhs.Belongs;
			else return &(*p)==&(*rhs.p);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return !(*this==rhs);}
		bool operator!=(const const_iterator &rhs) const {return !(*this==rhs);}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {return p->val;}
	};
 
	class const_iterator {
		friend class linked_hashmap;
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			// data members.
			const node *p;
			const linked_hashmap *Belongs;
		public:
			const_iterator() :p(nullptr),Belongs(nullptr){}
			const_iterator(const node *_p,const linked_hashmap *_Belongs):p(_p),Belongs(_Belongs){}
			const_iterator(const const_iterator &other) {p=other.p,Belongs=other.Belongs;}
			const_iterator(const iterator &other) {p=other.p,Belongs=other.Belongs;}
			// And other methods in iterator.
			// And other methods in iterator.
			// And other methods in iterator.
			/**
			* TODO iter++
			*/
			const_iterator operator++(int) {
				if(p==nullptr)throw invalid_iterator();
				const_iterator tmp=*this;
				p=p->time_next;
				return tmp;
			}
			/**
			* TODO ++iter
			*/
			const_iterator & operator++() {
				if(p==nullptr)throw invalid_iterator();
				p=p->time_next;
				return *this;
			}
			/**
			* TODO iter--
			*/
			const_iterator operator--(int) {
				if(p==Belongs->Fir)throw invalid_iterator();
				const_iterator tmp=*this;
				p=(p==nullptr?Belongs->Las:p->time_pre);
				return tmp;
			}
			/**
			* TODO --iter
			*/
			const_iterator & operator--() {
				if(p==Belongs->Fir)throw invalid_iterator();
				p=(p==nullptr?Belongs->Las:p->time_pre);
				return *this;
			}
			/**
			* a operator to check whether two iterators are same (pointing to the same memory).
			*/
			value_type & operator*() const {return *(p->val);}
			bool operator==(const iterator &rhs) const {
				if(p==nullptr||rhs.p==nullptr)return p==nullptr&&rhs.p==nullptr&&Belongs==rhs.Belongs;
				else return &(*p)==&(*rhs.p);
			}
			bool operator==(const const_iterator &rhs) const {
				if(p==nullptr||rhs.p==nullptr)return p==nullptr&&rhs.p==nullptr&&Belongs==rhs.Belongs;
				else return &(*p)==&(*rhs.p);
			}
			/**
			* some other operator for const_iterator.
			*/
			bool operator!=(const iterator &rhs) const {return !(*this==rhs);}
			bool operator!=(const const_iterator &rhs) const {return !(*this==rhs);}

			value_type* operator->() const noexcept {return p->val;}
	};
 
	/**
	 * TODO two constructors
	 */
	linked_hashmap() :Capacity(10),Fir(nullptr),Las(nullptr),Size(0){
		Point=new node*[Capacity];
		for(int i=0;i<Capacity;i++)Point[i]=nullptr;
	}
	linked_hashmap(const linked_hashmap &other){
		Capacity=other.Capacity,Size=other.Size;
		Point=new node*[Capacity];
		node **Point_las=new node*[Capacity];
		for(int i=0;i<Capacity;i++)Point[i]=nullptr;
		for(int i=0;i<Capacity;i++)Point_las[i]=nullptr;
		Fir=nullptr,Las=nullptr;
		for(node *x=other.Fir,*y=nullptr,*z;x!=nullptr;x=x->time_next){
			int H=Hash()(x->val->first)%Capacity;
			z=new node(*(x->val));
			if(Point_las[H]==nullptr)Point[H]=z;
			else Point_las[H]->next=z,z->pre=Point_las[H];
			Point_las[H]=z;
			if(y!=nullptr)z->time_pre=y,y->time_next=z;
			y=z;
			if(x==other.Fir)Fir=z;
			if(x==other.Las)Las=z;
		}
		delete[] Point_las;
	}

	void CC_clear(){
		node *x=Fir,*y;
		while(x!=nullptr){
			y=x->time_next;
			delete x;
			x=y;
		}
		Fir=Las=nullptr;
		delete[] Point;
	}
 
	/**
	 * TODO assignment operator
	 */
	linked_hashmap & operator=(const linked_hashmap &other) {
		if(this==&other)return *this;
		CC_clear();

		Capacity=other.Capacity,Size=other.Size;
		Point=new node*[Capacity];
		node **Point_las=new node*[Capacity];
		for(int i=0;i<Capacity;i++)Point[i]=nullptr;
		for(int i=0;i<Capacity;i++)Point_las[i]=nullptr;
		Fir=nullptr,Las=nullptr;
		for(node *x=other.Fir,*y=nullptr,*z;x!=nullptr;x=x->time_next){
			int H=Hash()(x->val->first)%Capacity;
			z=new node(*(x->val));
			if(Point_las[H]==nullptr)Point[H]=z;
			else Point_las[H]->next=z,z->pre=Point_las[H];
			Point_las[H]=z;
			if(y!=nullptr)z->time_pre=y,y->time_next=z;
			y=z;
			if(x==other.Fir)Fir=z;
			if(x==other.Las)Las=z;
		}
		delete[] Point_las;

		return *this;
	}
 
	/**
	 * TODO Destructors
	 */
	~linked_hashmap() {
		CC_clear();
	}
 
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
		int H=Hash()(key)%Capacity;
		for(node* x=Point[H];x!=nullptr;x=x->next){
			if( Equal()((*(x->val)).first,key) )return (*(x->val)).second;
		}
		throw index_out_of_bound();
	}
	const T & at(const Key &key) const {
		int H=Hash()(key)%Capacity;
		for(node* x=Point[H];x!=nullptr;x=x->next){
			if( Equal()((*(x->val)).first,key) )return (*(x->val)).second;
		}
		throw index_out_of_bound();
	}
 
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
		int H=Hash()(key)%Capacity;
		for(node* x=Point[H];x!=nullptr;x=x->next){
			if( Equal()((*(x->val)).first,key) )return (*(x->val)).second;
		}
		iterator it=insert(value_type(key,T())).first;
		return it->second;
	}
 
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
		int H=Hash()(key)%Capacity;
		for(node* x=Point[H];x!=nullptr;x=x->next){
			if( Equal()((*(x->val)).first,key) )return (*(x->val)).second;
		}
		throw index_out_of_bound();
	}
 
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {return iterator(Fir,this);}
	const_iterator cbegin() const {return const_iterator(Fir,this);}
 
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {return iterator(nullptr,this);}
	const_iterator cend() const {return const_iterator(nullptr,this);}
 
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {return Size==0;}
 
	/**
	 * returns the number of elements.
	 */
	size_t size() const {return Size;}
 
	/**
	 * clears the contents
	 */
	void clear() {
		CC_clear();
		Capacity=10,Fir=Las=nullptr,Size=0;
		Point=new node*[10];
		for(int i=0;i<Capacity;i++)Point[i]=nullptr;
	}
 
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	void DoubleSpace(){
		delete[] Point;
		Capacity<<=1;
		Point=new node*[Capacity];
		node **Point_las=new node*[Capacity];
		for(int i=0;i<Capacity;i++)Point[i]=nullptr;
		for(int i=0;i<Capacity;i++)Point_las[i]=nullptr;
		for(node *x=Fir,*y=nullptr,*z;x!=nullptr;x=x->time_next){
			int H=Hash()(x->val->first)%Capacity;
			x->pre=x->next=nullptr;
			if(Point_las[H]==nullptr)Point[H]=x;
			else Point_las[H]->next=x,x->pre=Point_las[H];
			Point_las[H]=x;
		}
		delete[] Point_las;
	}
	void DivSpace(){
		delete[] Point;
		Capacity>>=1;
		Point=new node*[Capacity];
		node **Point_las=new node*[Capacity];
		for(int i=0;i<Capacity;i++)Point[i]=nullptr;
		for(int i=0;i<Capacity;i++)Point_las[i]=nullptr;
		for(node *x=Fir,*y=nullptr,*z;x!=nullptr;x=x->time_next){
			int H=Hash()(x->val->first)%Capacity;
			x->pre=x->next=nullptr;
			if(Point_las[H]==nullptr)Point[H]=x;
			else Point_las[H]->next=x,x->pre=Point_las[H];
			Point_las[H]=x;
		}
		delete[] Point_las;
	}
	pair<iterator, bool> insert(const value_type &value) {
		if(Size>Capacity*LoadFactor)DoubleSpace();

		int H=Hash()(value.first)%Capacity;
		// std::cout<<"!!!"<<H<<std::endl;
		node *y=nullptr;
		for(node* x=Point[H];x!=nullptr;x=x->next){
			if( Equal()(x->val->first,value.first) )return pair<iterator, bool> (iterator(x,this),0);
			y=x;
		}
		// std::cout<<"!!!"<<H<<std::endl;
		node *z=new node(value);
		if(y==nullptr){
			Point[H]=z;
		}
		else y->next=z,z->pre=y;
		if(Size)Las->time_next=z,z->time_pre=Las,Las=z;
		else Fir=Las=z;
		Size++;
		return pair<iterator, bool>(iterator(z,this),1);
	}
 
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
		if(Size>10&&Size<Capacity*LoadFactor2)DivSpace();

		if(pos.Belongs!=this||pos==end())throw invalid_iterator();
		node *x=pos.p;
		if(x->pre!=nullptr)x->pre->next=x->next;
		else {
			int H=Hash()(x->val->first)%Capacity;
			Point[H]=x->next;
		}
		if(x->next!=nullptr)x->next->pre=x->pre;

		if(x->time_pre!=nullptr)x->time_pre->time_next=x->time_next;
		else Fir=x->time_next;
		if(x->time_next!=nullptr)x->time_next->time_pre=x->time_pre;
		else Las=x->time_pre;
		Size--;
		delete x;
	}
 
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 */
	size_t count(const Key &key) const {
		int H=Hash()(key)%Capacity;
		for(node* x=Point[H];x!=nullptr;x=x->next){
			if( Equal()((*(x->val)).first,key) )return 1;
		}
		return 0;
	}
 
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
		int H=Hash()(key)%Capacity;
		for(node* x=Point[H];x!=nullptr;x=x->next){
			if( Equal()((*(x->val)).first,key) )return iterator(x,this);
		}
		return end();
	}
	const_iterator find(const Key &key) const {
		int H=Hash()(key)%Capacity;
		for(node* x=Point[H];x!=nullptr;x=x->next){
			if( Equal()(x->val->first,key) )return const_iterator(x,this);
		}
		return cend();
	}
};

}

#endif
