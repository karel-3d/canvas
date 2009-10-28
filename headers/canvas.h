#ifndef CANVAS_INC
#define CANVAS_INC

#include "matrix.h"
#include "gr_object.h"

namespace glib {
	class canvas {
	public:
		typedef std::list<gr_object*> gr_objects; //list vsech grafickych objektu
		typedef plane<RGBa>::T_list colors_row;	//list struktur (barva, zacatek, konec) z plane
	private:
		
		bool _antialias;
		glib_int _height;
		glib_int _width; //mohlo by byt uint, ale to bych tam stejne nic nikdy nenakreslil
		                
		RGBa _background;
		
		gr_objects _gr_objects;
		
	public:
		glib_int get_width() const;
		glib_int get_height() const;
		
		canvas(const size_t width, const size_t height, const RGBa& background, bool antialias);
		canvas();
		
		virtual ~canvas();
		
		matrix<glib_component> get_matrix(const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos); 
		plane<RGBa> get_plane();
			//NENI CONST
			//sou to sice gettery, ale az ty gettery rikaji potomkum "hele, nakreslete se, kthx"!
		
		void remove(const size_t pos);
		void pop_back();
		void pop_front();
		void push_front(const gr_object* const g);
		void push_back(const gr_object* const g);
		void push_front(const gr_object* const g, size_t pos);
		void push_back(const gr_object* const g, size_t pos);
		void remove_all();
		gr_object* get_object(size_t pos) const;
	};
}

#endif

