#include "Color.h"
#include <math.h>
#include <iostream>

// The default constructor.
LIB2D::Color::Color()
{
	m_red = 0.0;
	m_green = 0.0;
	m_blue = 0.0;
	m_hue = 0.0;
	m_saturation = 0.0;
	m_value = 0.0;
}

// The specific constructor.
LIB2D::Color::Color(double r, double g, double b)
{
	m_red = r;
	m_green = g;
	m_blue = b;
	ComputeHSV();
}

// Methods to set the color.
void LIB2D::Color::SetRGB(const unsigned char r, const unsigned char g, const unsigned char b)
{
	m_red = static_cast<double>(r) / 255.0;
	m_green = static_cast<double>(g) / 255.0;
	m_blue = static_cast<double>(b) / 255.0;
	ComputeHSV();
}

void LIB2D::Color::SetHSV(const unsigned char h, const unsigned char s, const unsigned char v)
{
	m_hue = (static_cast<double>(h) / 255.0) * 360.0;
	m_saturation = static_cast<double>(s) / 255.0;
	m_value = static_cast<double>(v) / 255.0;
	ComputeRGB();
}

void LIB2D::Color::SetRGB(double r, double g, double b)
{
	m_red = r;
	m_green = g;
	m_blue = b;
	ComputeHSV();
}

void LIB2D::Color::SetHSV(double h, double s, double v)
{
	m_hue = h;
	m_saturation = s;
	m_value = v;
	ComputeRGB();
}

// Methods to retrieve the color.
unsigned char LIB2D::Color::GetRed()
{
	return static_cast<unsigned char>(m_red * 255.0);
}
unsigned char LIB2D::Color::GetGreen()
{
	return static_cast<unsigned char>(m_green * 255.0);
}
unsigned char LIB2D::Color::GetBlue()
{
	return static_cast<unsigned char>(m_blue * 255.0);
}
unsigned char LIB2D::Color::GetHue()
{
	return static_cast<unsigned char>((m_hue / 360.0) * 255.0);
}
unsigned char LIB2D::Color::GetSaturation()
{
	return static_cast<unsigned char>(m_saturation * 255.0);
}
unsigned char LIB2D::Color::GetValue()
{
	return static_cast<unsigned char>(m_value * 255.0);
}

double LIB2D::Color::GetRedf()
{
	return m_red;
}
double LIB2D::Color::GetGreenf()
{
	return m_green;
}
double LIB2D::Color::GetBluef()
{
	return m_blue;
}
double LIB2D::Color::GetHuef()
{
	return m_hue;
}
double LIB2D::Color::GetSaturationf()
{
	return m_saturation;
}
double LIB2D::Color::GetValuef()
{
	return m_value;
}

// Methods to convert between colour spaces.

void LIB2D::Color::ComputeHSV()
{
	/* Convert from RGB to HSV.
	
		First, determine the maximum and minimum values
		and which colours (red, green or blue) that they
		correspond to.
	*/
	double minValue, maxValue;
	unsigned char maxIndex;
	if ((m_red == m_green) && (m_red == m_blue))
	{
		maxIndex = 0;
		maxValue = m_red;
		minValue = m_red;
	}
	else if ((m_red >= m_green) && (m_red >= m_blue))
	{
		maxIndex = 1;
		maxValue = m_red;
		minValue = (m_green < m_blue) ? m_green : m_blue;
	}
	else if ((m_green >= m_red) && (m_green >= m_blue))
	{
		maxIndex = 2;
		maxValue = m_green;
		minValue = (m_red < m_blue) ? m_red : m_blue;
	}
	else
	{
		maxIndex = 3;
		maxValue = m_blue;
		minValue = (m_red < m_green) ? m_red : m_green;
	}
	
	// Now we can compute the value of h.
	double h;
	switch(maxIndex)
	{
		case 0:	h = 0.0;
						break;
		case 1: h = 60.0 * ((m_green - m_blue) / (maxValue - minValue));
						break;
		case 2: h = 60.0 * (2 + ((m_blue - m_red) / (maxValue - minValue)));
						break;
		case 3: h = 60.0 * (4 + ((m_red - m_green) / (maxValue - minValue)));
						break;
	}
	if (h < 0.0)
		h += 360.0;
		
	// Compute the saturation.
	double s;
	s = (maxIndex == 0) ? 0.0 : ((maxValue - minValue) / maxValue);
	
	// Compute the value.
	double v = maxValue;
	
	// And store the result.
	m_hue = h;
	m_saturation = s;
	m_value = v;						
}

void LIB2D::Color::ComputeRGB()
{
	// Convert HSV values to RGB.
	double rgbRange = m_value * m_saturation;
	double maxRGB = m_value;
	double minRGB = m_value - rgbRange;
	double hPrime = m_hue / 60.0;
	double x1 = fmod(hPrime, 1.0);
	double x2 = 1.0 - fmod(hPrime, 1.0);
	
	if ((hPrime >= 0) && (hPrime < 1))
	{
		m_red = maxRGB;
		m_green = (x1 * rgbRange) + minRGB;
		m_blue = minRGB;
	}
	else if ((hPrime >= 1) && (hPrime < 2))
	{
		m_red = (x2 * rgbRange) + minRGB;
		m_green = maxRGB;
		m_blue = minRGB;
	}
	else if ((hPrime >= 2) && (hPrime < 3))
	{
		m_red = minRGB;
		m_green = maxRGB;
		m_blue = (x1 * rgbRange) + minRGB;
	}
	else if ((hPrime >= 3) && (hPrime < 4))
	{
		m_red = minRGB;
		m_green = (x2 * rgbRange) + minRGB;
		m_blue = maxRGB;
	}
	else if ((hPrime >= 4) && (hPrime < 5))
	{	
		m_red = (x1 * rgbRange) + minRGB;
		m_green = minRGB;
		m_blue = maxRGB;
	}
	else if ((hPrime >= 5) && (hPrime < 6))
	{
		m_red = maxRGB;
		m_green = minRGB;
		m_blue = (x2 * rgbRange) + minRGB;
	}
	else
	{
		m_red = 0.0;
		m_green = 0.0;
		m_blue = 0.0;
	}
	
}