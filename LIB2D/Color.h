#pragma once

namespace LIB2D
{
	class Color
	{

	public:
		// The default constructor.
		Color();
		
		// A specific constructor.
		Color(double r, double g, double b);
		
		// Methods for setting the color.
		void SetRGB(const unsigned char r, const unsigned char g, const unsigned char b);
		void SetHSV(const unsigned char h, const unsigned char s, const unsigned char v);
		
		void SetRGB(double r, double g, double b);
		void SetHSV(double h, double s, double v);
		
		// Methods for retrieving the color.
		unsigned char GetRed();
		unsigned char GetGreen();
		unsigned char GetBlue();
		
		unsigned char GetHue();
		unsigned char GetSaturation();
		unsigned char GetValue();
		
		double GetRedf();
		double GetGreenf();
		double GetBluef();
		
		double GetHuef();
		double GetSaturationf();
		double GetValuef();

	private:
		double m_red;
		double m_green;
		double m_blue;
		
		double m_hue;
		double m_saturation;
		double m_value;
		
		void ComputeHSV();
		void ComputeRGB();

	};
}