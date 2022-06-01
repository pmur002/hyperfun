/*==============================================================================

 Copyright 1999 Eric Fausett
 Copyright 2003-2004 Benjamin Schmitt 

 This Work or file is part of the greater total Work, software or group of
 files named HyperFun Polygonizer.

 The implemented polygonization algorithm is described in

 Pasko A.A., Pilyugin V.V., Pokrovskiy V.N. 
 "Geometric modeling in the analysis of trivariate functions", 
 Communications of Joint Insititute of Nuclear Research, P10-86-310, 
 Dubna, Russia, 1986 (in Russian).

 Pasko A.A., Pilyugin V.V., Pokrovskiy V.N. 
 "Geometric modeling in the analysis of trivariate functions",
 Computers and Graphics, vol.12, Nos.3/4, 1988, pp.457-465. 

 HyperFun Polygonizer can be redistributed and/or modified under the terms 
 of the CGPL, The Common Good Public License as published by and at CGPL.org
 (http://CGPL.org).  It is released under version 1.0 Beta of the License
 until the 1.0 version is released after which either version 1.0 of the
 License, or (at your option) any later version can be applied.

 THIS WORK, OR SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED (See the
 CGPL, The Common Good Public License for more information.)

 You should have received a copy of the CGPL along with HyperFun Polygonizer;  
 if not, see -  http://CGPL.org to get a copy of the License.

==============================================================================*/

// HFTimer.h: interface for the HFTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(HFTIMER_H)
#define HFTIMER_H

#include <time.h>
#include <string>
#include <iostream>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class HFTimer{
	public:
		void stop();
		void start(std::string str);
		
		HFTimer();
		virtual ~HFTimer();


	private:
		clock_t itsStart;
		clock_t itsFinish;
		std::string itsString;
		
	};

#endif // !defined(HFTIMER_H)
