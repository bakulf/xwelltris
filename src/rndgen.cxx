// docm_prefix(///)
/****************************************************************************
*  Copyright (C) 2002 by Leo Khramov
*  email:   leo@xnc.dubna.su
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
 ****************************************************************************/
// $Id: rndgen.cxx,v 1.1.1.1 2003/01/04 11:37:22 leo Exp $

/// module description
/// Dr. Park's algorithm published in the Oct. '88 ACM 
/// "Random Number Generators: Good Ones Are Hard To Find"
/// His version available at ftp://cs.wm.edu/pub/rngs.tar
/// Present form by many authors.


static int Seed = 1;       /* This is required to be 32 bits long */

//===========================================================================
/// global SetRNG(long)
///     Set random seed generator,
///     Given an integer, this routine initializes the RNG seed.
/// tags random_gen
void SetRNG(long s)
{
  Seed = (int) s;
}

//===========================================================================
/// global LongRNG()
///      Returns an pseudo random integer between 0 and 2147483647, inclusive.
/// tags random_gen
long LongRNG()
{
  if ((Seed = Seed % 44488 * 48271 - Seed / 44488 * 3399) < 0)
    Seed += 2147483647;
  return (long) (Seed - 1);
}
