#ifndef PL_INC
#define PL_INC

#include "interval.h" //kazdy radek je interval

#define __real_height (_end_height - _start_height)

namespace canlib {

	template<class T>
	class plane {
		/* 
		 * Trida na ukladani pixelu, co nezabira zbytecne mnoho mista a pritom je pomerne rychla.
		 * Kazdy radek je ulozen jako "interval" - struktura na ukladani intervalu - vice info o te strukture tam.
		 * Radky pak jsou ulozeny v "obycejnem" vektoru.
		 *
		 * Nevyhoda je v tom, ze napriklad u obdelniku bude v kazdem radku totez, ty same intervaly. Na
		 * druhou stranu, je pak jednoduche smichat vic ploch dohromady - s tim, ze smichat
		 * dva radky s intervaly je jednoduche, kdezto nejaka struktura pamatujuci si vylozene 2D plochy
		 * by tohle mela o dost komplikovanejsi, stejne jako pridavani "po pixelech".
		 *
		 * Radky jsou cislovany relativne - tim je mysleno, ze vektor je sice indexovan od nuly, ale 
		 * navenek se plocha tvari, ze je indexovana od jakehokoliv radku po jakykoliv, tj. klidne i v zapornych cislech - 
		 * hodnota prvniho a posledniho radku ale musi byt znama uz pri vytvareni konstruktorem.
		 *
		 * vic o intervalech v interval.h (snad)
		 */
	public:
		typedef interval<T>* T_intervals;
		
	private:	
		
		
		canlib_int _start_height; //zacatecni vyska
		canlib_int _end_height; //konecna vyska
		
		interval<T>* _intervals;
		
	public:
		canlib_int _pivot_width; 
		canlib_int _pivot_height;
		
		typedef std::list<interval_content<T> > T_list;
			//list "obsahu" - obsah struktura T, zacatek, konec
		
		//typedef std::list<interval_content<bool> > bool_list;
			//totez special pro bool - hodi se pro jeden konstruktor
		
		plane();
		
		plane(const plane<T>& other);
		
		plane(const canlib_int start_height, const canlib_int end_height, const canlib_int pivot_width=0);
			//konstruktor, co chce pocatecni a konecnou vysku a pivotni sirku a neprida nikam nic
		
		plane(const canlib_int start_width, const canlib_int end_width, const canlib_int start_height, const canlib_int end_height, const T& what);
			//konstruktor, co se mu jeste da zacatek a konec kazdeho radku a on do kazdeho prida velky interval s danou hodnotou
	
	
		~plane();
	
		template <class U>
		plane<U> flatten_plane(const U& what, const T& min) const;
		
		canlib_int get_start_height() const;
		canlib_int get_end_height() const;
		canlib_int get_real_height() const;
		T_intervals get_intervals() const;
		
		canlib_int get_pivot_height() const {return _pivot_height;}
		canlib_int get_pivot_width() const {return _pivot_width;}
		
			//klasicke gettery
		
		canlib_int first_non_zero() const;
		canlib_int last_non_zero() const;
		
		canlib_int most_left(canlib_int y) const;
		canlib_int most_right(canlib_int y) const;
		
		canlib_int most_top_left() const;
		canlib_int most_down_right() const;
		
		canlib_int most_left() const;
		canlib_int most_right() const;
		
		
		T_list get_row(const canlib_int y) const;
			//vrati konkretni radek v listu paru <T, rozsah>
			
		void set_whole_interval(const canlib_int y, const interval<T> new_interval);
			//vlaste pomerne troufale, ale co :)
		
		T get(const canlib_int x, const canlib_int y) const;
			//vrati hodnotu na konkretni pozici, nebo defaultni T(), pokud tam nic neni
		
		void add(const canlib_int x, const canlib_int y, const T& what);
			//nastavi konkretni hodnotu (pozor, SCITA!)
		
		void add_more(const canlib_int start_x, const canlib_int end_x, const canlib_int y, const T& what);
			//nastavi na y radku vse od start_x do end_x na what (tj, prida interval, mozna neco upravi)
		
		plane<T> move(const canlib_int pivot_width, const canlib_int start_height);
			//posune cely plane, aby mel "novy" pivot_width a start_height
			//pozor, sam se sebou neposune, ale posunty VRACI
		
		void add(const plane<T>& other);
			//slouci s jinou plochou 
			//POZOR - z jine plochy vezme jen veci od _start_height po _end_height, zbytek ignoruje
			
		plane<T>& operator=(const plane<T>& other);
		
		bool includes_square(const canlib_int min_x, const canlib_int min_y, const canlib_int max_x, const canlib_int max_y) const;
	};


	//--------------------------------CONSTRUCTORS
	template<class T>
	plane<T>::plane() :
	  _start_height(0),
	  _end_height(0),
	  _intervals(NULL),
	  _pivot_width(0),
	  _pivot_height(0) {
	}
	
	template<class T>
	plane<T>::plane(const canlib_int start_height, const canlib_int end_height, const canlib_int pivot_width) :
	  _start_height(start_height), 
	  _end_height(__maximum(start_height, end_height)),	
	  _intervals(new interval<T>[__real_height]),
	  _pivot_width(pivot_width),
	  _pivot_height(0){
	}
	
	template<class T>
	plane<T>::plane(const canlib::plane<T>& other) :
	  _start_height(other._start_height), 
	  _end_height(other._end_height),	
	  _intervals(new interval<T>[__real_height]),
	  _pivot_width(other._pivot_width),
	  _pivot_height(other._pivot_height) {
		
		canlib_int rs = __real_height;
		for (canlib_int i = 0; i < rs; ++i) {
			_intervals[i]=other._intervals[i];
		}
		
	}
	
	
	
	
		//cely zaplni whatem
	template<class T>
	plane<T>::plane(const canlib_int start_width, const canlib_int end_width, const canlib_int start_height, const canlib_int end_height, const T& what) : 
	  _start_height(start_height),
	  _end_height(__maximum(start_height, end_height)), 
	  _intervals(new interval<T>[__real_height]),
	  _pivot_width(0),
	  _pivot_height(0) {
		
		canlib_int rs = __real_height;
		for (canlib_int i = 0; i < rs; ++i) {
			_intervals[i]=interval<T>(start_width, end_width-1, what);
		}
	}
	
	//-------------------------------DESTRUCTORS
	template<class T>
	plane<T>::~plane() {
		delete [] _intervals; 
	}
	
	//--------------------------------GETTERS
	template <class T> 
	canlib_int
	plane<T>::most_left() const {
		canlib_int min = canlib_INT_MAX;
		for (canlib_int i = first_non_zero(); i <= last_non_zero(); ++i) {
			min = __minimum(min, _intervals[i-_start_height].most_left());
		}
		return min;
	}

	template <class T> 
	canlib_int
	plane<T>::most_right() const {
		canlib_int max = 0;
		for (canlib_int i = first_non_zero(); i <= last_non_zero(); ++i) {
			max = __maximum(max, _intervals[i-_start_height].most_right());
		}
		return max;
	}

	template <class T>
	canlib_int
	plane<T>::most_top_left() const {
		return most_left(first_non_zero());
	
	}


	template <class T> 
	canlib_int
	plane<T>::most_down_right() const {
		return most_right(last_non_zero());
	}

	template <class T> 
	canlib_int
	plane<T>::most_left(canlib_int y) const {
		return _intervals[y-_start_height].most_left();
	}

	template <class T> 
	canlib_int
	plane<T>::most_right(canlib_int y) const {
		return _intervals[y-_start_height].most_right();
	}

	template <class T> 
	canlib_int
	plane<T>::first_non_zero() const {
		for (canlib_int i = 0; i < __real_height; ++i) {
			if (!_intervals[i].is_empty())
				return i+_start_height;
		}
		return _end_height;
	}

	template <class T> 
	canlib_int
	plane<T>::last_non_zero() const {
		if (__real_height!=0) {
			for (canlib_uint i = __real_height - 1; i != 0; --i) {
				if (!_intervals[i].is_empty())
					return i+_start_height;
			}
			return _start_height-1;
		} else {
			return 0;
		}
	}

	template <class T> 
	canlib_int
	plane<T>::get_start_height() const {
		return _start_height;
	}

	template <class T> 
	canlib_int
	plane<T>::get_real_height() const {
		return __real_height;
	}

	template <class T> canlib_int
	plane<T>::get_end_height() const {
		return _end_height;
	}

	
	template <class T> 
	typename plane<T>::T_intervals
	plane<T>::get_intervals() const {
		return _intervals;
	}
	
	
	template<class T> 
	typename plane<T>::T_list 
	plane<T>::get_row(canlib_int y) const {
		if ((y >= _end_height) || (y < _start_height)) {
			return T_list();
		}
		return _intervals[y-_start_height].get_all();
			//opet, odolnost proti chybam, jinak se vse prenasi na intervals[]
	}
	
	template<class T>
	bool
	plane<T>::includes_square(const canlib_int min_x, const canlib_int min_y, const canlib_int max_x, const canlib_int max_y) const {
		if (min_y < _start_height) {
			return false;
		}
		
		if (max_y > _end_height) {
			return false;
		}
		
		for (int y = min_y; y < max_y; ++y) {
			if (!_intervals[y-_start_height].includes(min_x, max_x)) {
				return false;
			}
		}
		
		return true;
	}
	
	//--------------------------------------SETTERS
	
	template<class T>
	plane<T>&
	plane<T>::operator=(const canlib::plane<T>& other) {
		_pivot_width = other._pivot_width;
		_pivot_height = other._pivot_height;
		
		_start_height = other._start_height;
		_end_height=other._end_height;
		delete [] _intervals;
		
		canlib_int rs = __real_height;
		_intervals=new interval<T>[__real_height];
		
		for (canlib_int i = 0; i < rs; ++i) {
			_intervals[i]=other._intervals[i];
		}
		return *this;
	}
	
	template<class T>
	void 
	plane<T>::set_whole_interval(const canlib_int y, const interval<T> new_interval) {
		if (y<=__real_height) {
			_intervals[y]=new_interval;
			//tohle trva, bohuzel, pomerne dlouho
		}
	}
	
	template<class T> 
	void 
	plane<T>::add(const canlib_int x, const canlib_int y, const T& what) {
		if ((y >= _end_height) || (y < _start_height)) {
			return;
		}
			//odolne proti chybam - prijde mi to uzitecnejsi
		_intervals[y-_start_height].add_one(x, what);
	
	}

	template<class T> void  
	canlib::plane<T>::add_more(const canlib::canlib_int start_x, const canlib::canlib_int end_x, const canlib::canlib_int y, const T& what) {
		if ((y >= _end_height) || (y < _start_height)) {
			return;
		}
		if (end_x >= start_x)
			_intervals[y-_start_height].add_more(start_x, end_x, what);
			//opet, pri zadani blbosti se nestane nic
	}

	template<class T> void  
	canlib::plane<T>::add(const plane<T>& other) {
	
		canlib_int start = __maximum(_start_height, other._start_height);
		canlib_int end = __minimum(_end_height, other._end_height);
			//start a end jsou minimalni hranice, co musim zkoumat
			//POZOR - radky, co jsou nad this->_start_height a pod this->_end_height se ignoruji!
	
		for (canlib_int i = start; i < end; ++i) {
			canlib_uint it = i - _start_height;
			canlib_uint oth_it = i - other._start_height;
				//beru primo z vektoru, bez get_row, musim vedet jak svuj index (it) tak index toho druheho (oth_it)

			_intervals[it].add_another(other._intervals[oth_it]);
		
		}
	}
	
	
	template<class T> T 
	plane<T>::get(const canlib_int x, const canlib_int y) const {
		if ((y >= _end_height) || (y < _start_height)) {
			return T();
		}
		return _intervals[y-_start_height].get(x);
			//vsechno prenasi jinam
	}
	
	//----------------------------------------OTHER
	template <class T> 
	plane<T>
	plane<T>::move(const canlib_int new_width, const canlib_int new_height) {
		plane<T> res = plane<T>(*this);
			//neni overloadnuty CC - neni se tu ceho bat, neni tu zadne pole
			//(stromova struktura intervals sama sebe umi zkopirovat)
	
		
		canlib_int height_diff = (new_height - _pivot_height); //- kdyz nahoru, + kdyz dolu
		canlib_int width_diff = (new_width - _pivot_width);
				
		res._start_height = res._start_height + height_diff;
		res._end_height = res._end_height + height_diff;
			//posouvame vysku
		
		res._pivot_width = new_width;
		res._pivot_height = new_height;
	
		for (canlib_int i = 0; i < __real_height; ++i) {
			res._intervals[i].move(-width_diff);
		}
			//posouvame sirku
		return res; //ta-taa
	}
	
	
	template<class T>
	template<class U>   
	plane<U>
	plane<T>::flatten_plane(const U& what, const T& min) const {
		
		plane<U> result(_start_height, _end_height, _pivot_width);
		
		int rs = __real_height;
		for (int i=0; i<rs; ++i) {
			interval<U>* p_int = (_intervals[i]).template flatten_interval<U>(what, min);
			result.set_whole_interval(i, *p_int);
			delete p_int;
				//tohle vypada hrozne, ale tady by mel ve vetsine pripadu vylezt interval,
				// co bude mit velmi malo potomku, jestli vubec nejake
		}
		
		return result;
	}
	

	




}

#endif
