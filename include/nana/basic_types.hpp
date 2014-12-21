/*
 *	Basic Types definition
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2014 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/basic_types.hpp
 */

#ifndef NANA_BASIC_TYPES_HPP
#define NANA_BASIC_TYPES_HPP

#include <nana/deploy.hpp>

namespace nana
{
	//A constant value for the invalid position.
	const std::size_t npos = static_cast<std::size_t>(-1);


	template<typename CharT>
	struct casei_char_traits
		: public std::char_traits<CharT>
	{
		typedef CharT char_type;

		
		//static constexpr bool eq(char_type c1, char_type c2) noexcept
		//VC2012 does not support constexpr and noexcept keywords
		static bool eq(char_type c1, char_type c2)
		{
			return std::toupper(c1) == std::toupper(c2);
		}

		//static constexpr bool lt(char_type c1, char_type c2) noexcept
		//VC2012 does not support constexpr and noexcept keywords
		static bool lt(char_type c1, char_type c2)
		{
			return std::toupper(c1) < std::toupper(c2);
		}

		static int compare(const char_type* s1, const char_type* s2, std::size_t n)
		{
			while(n--)
			{
				char_type c1 = std::toupper(*s1);
				char_type c2 = std::toupper(*s2);
				if(c1 < c2) return -1;
				if(c1 > c2) return 1;
				++s1;
				++s2;
			}
			return 0;
		}

		static const char_type* find(const char_type* s, std::size_t n, const char_type& a)
		{
			char_type ua = std::toupper(a);
			const char_type * end = s + n;
			while((s != end) && (std::toupper(*s) != ua))
				++s;
			return (s == end ? nullptr : s);
		}
	};

	typedef std::basic_string<nana::char_t, casei_char_traits<nana::char_t> > cistring;
	

	namespace detail
	{
		struct drawable_impl_type;	//declearation, defined in platform_spec.hpp
	}

	namespace paint
	{
		typedef nana::detail::drawable_impl_type*	drawable_type;
	}

	enum class mouse_action
	{
		begin, normal = begin, over, pressed, end
	};

	enum class element_state
	{
		normal,
		hovered,
		focus_normal,
		focus_hovered,
		pressed,
		disabled
	};

	typedef unsigned scalar_t;
	typedef unsigned char	uint8_t;
	typedef unsigned long	uint32_t;
	typedef unsigned		uint_t;
	typedef long long long_long_t;

	union pixel_argb_t
	{
		struct element_tag
		{
			unsigned char blue;
			unsigned char green;
			unsigned char red;
			unsigned char alpha_channel;
		}element;
		unsigned value;
	};

	union pixel_rgba_t
	{
		struct element_tag
		{
			unsigned char alpha_channel;
			unsigned char blue;
			unsigned char green;
			unsigned char red;
		}element;
		unsigned value;
	};

	using pixel_color_t = pixel_argb_t;

	//http://www.w3.org/TR/2011/REC-css3-color-20110607/
	//4.3. Extended color keywords
	enum class colors
	{
		black	= 0x0,
		blue	= 0x0000FF,

		gray	= 0x808080,
		green	= 0x00FF00,
		red		= 0xFF0000,
		white	= 0xFFFFFF,

		//temporary defintions, these will be replaced by color schema
		button_face_shadow_start = 0xF5F4F2,
		button_face_shadow_end = 0xD5D2CA,
		button_face = 0xD4D0C8,
		dark_border = 0x404040,
		gray_border = 0x808080,
		highlight = 0x1CC4F7
	};

	//Some helper types to identify an integer as color.
	enum class color_rgb :	unsigned{};
	enum class color_argb:	unsigned{};
	enum class color_rgba : unsigned{};

	class expr_color
	{
	public:
		expr_color() = default;
		expr_color(colors);
		expr_color(colors, double alpha);
		expr_color(color_rgb);
		expr_color(color_argb);
		expr_color(color_rgba);
		expr_color(unsigned red, unsigned green, unsigned blue);
		expr_color(unsigned red, unsigned green, unsigned blue, double alpha);

		expr_color& alpha(double);	///< Sets alpha channel
		expr_color& from_rgb(unsigned red, unsigned green, unsigned blue);		///< immutable alpha channel
		expr_color& from_hsl(double hue, double saturation, double lightness);	///< immutable alpha channel

		expr_color& blend(const expr_color& bgcolor, bool ignore_bgcolor_alpha);

		///< Blends two colors with the specified alpha, and the alpha values that come with these two colors are both ignored. 
		expr_color& blend(const expr_color& bgcolor, double alpha);

		bool invisible() const;
		pixel_color_t px_color() const;
		pixel_argb_t argb() const;
		pixel_rgba_t rgba() const;

		const double& r() const;
		const double& g() const;
		const double& b() const;
		const double& a() const;

		bool operator==(const expr_color& other) const;
		bool operator!=(const expr_color& other) const;
	private:
		double r_;
		double g_;
		double b_;
		double a_{ 0.0 };	//invisible
	};


	struct rectangle;

	struct point
	{
		point();
		point(int x, int y);
		point(const rectangle&);

		point& operator=(const rectangle&);
		bool operator==(const point&) const;
		bool operator!=(const point&) const;
		bool operator<(const point&) const;
		bool operator<=(const point&) const;
		bool operator>(const point&) const;
		bool operator>=(const point&) const;

		point operator-(const point&) const;
		point operator+(const point&) const;
		point& operator-=(const point&);
		point& operator+=(const point&);

		int x;
		int y;
	};

	struct upoint
	{
		typedef unsigned value_type;

		upoint();
		upoint(value_type x, value_type y);
		bool operator==(const upoint&) const;
		bool operator!=(const upoint&) const;
		bool operator<(const upoint&) const;
		bool operator<=(const upoint&) const;
		bool operator>(const upoint&) const;
		bool operator>=(const upoint&) const;

		value_type x;
		value_type y;
	};

	struct size
	{
		size();
		size(unsigned width, unsigned height);
		size(const rectangle&);

		size& operator=(const rectangle&);

		bool empty() const;		///< true if width * height == 0
		bool is_hit(const point&) const;	///< Assume it is a rectangle at (0,0), and check whether a specified position is in the rectange.
		bool operator==(const size& rhs) const;
		bool operator!=(const size& rhs) const;
		size operator+(const size&) const;

		unsigned width;
		unsigned height;
	};

	struct rectangle
	{
		rectangle();									 ///< a zero-size rectangle at (0, 0).
		rectangle(int x, int y, unsigned width, unsigned height);
		rectangle(const size &);						 ///< a rectangle with specified size at coordinate (0, 0).
		rectangle(const point&, const size& = size());

		bool operator==(const rectangle& rhs) const;
		bool operator!=(const rectangle& rhs) const;

		rectangle& operator=(const point&);
		rectangle& operator=(const size&);

		rectangle& set_pos(const point&);
		rectangle& set_size(const size&);

		rectangle& pare_off(int pixels);	 ///<Pares the specified pixels off the rectangle. It's equal to x += pixels; y + pixels; width -= (pixels << 1); height -= (pixels << 1);

		int right() const;
		int bottom() const;
		bool is_hit(int x, int y) const;
		bool is_hit(const point& pos) const;
		bool empty() const;		///< true if width * height == 0

		int x;
		int y;
		unsigned width;
		unsigned height;
	};

	class area_rotator
	{
	public:
		area_rotator(bool rotated, const ::nana::rectangle& area);

		int x() const;
		int & x_ref();
		int y() const;
		int & y_ref();
		unsigned w() const;
		unsigned & w_ref();
		unsigned h() const;
		unsigned & h_ref();

		int right() const;
		int bottom() const;
		const ::nana::rectangle& result() const;
	private:
		bool rotated_;
		::nana::rectangle area_;
	};//end class area_rotator

	enum class arrange
	{
		unknown, horizontal, vertical, horizontal_vertical
	};

	///The definition of horizontal alignment
	enum class align
	{
		left, center, right
	};

	///The definition of vertical alignment
	enum class align_v
	{
		top, center, bottom
	};
}//end namespace nana

#endif


