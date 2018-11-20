/*----------------------------------------------------------------------
FILE        : ByteConverter.java
AUTHOR      : Oguz Karan
LAST UPDATE : 26.01.2018

ByteConverter class for byte operations with built-in types

Copyleft (c) 1993 by C and System Programmers Association (CSD)
All Rights Free
-----------------------------------------------------------------------*/

package org.csystem.util;

import java.nio.ByteBuffer;

public class ByteConverter {
	public static byte[] getBytes(int value)
	{
		return ByteBuffer.allocate(4).putInt(value).array();
	}
	
	public static byte[] getBytes(long value)
	{
		return ByteBuffer.allocate(8).putLong(value).array();
	}
	
	public static byte[] getBytes(double value)
	{
		return ByteBuffer.allocate(8).putDouble(value).array();
	}
	
	public static byte[] getBytes(boolean value)
	{
		return new byte[] {(byte)(value ? 1 : 0)};
	}
	
	public static int toInt(byte [] data)
	{
		return toInt(data, 0);		
	}
	
	public static int toInt(byte [] data, int offset)
	{
		return ByteBuffer.wrap(data, offset, 4).getInt();		
	}
	
	public static long toLong(byte [] data)
	{
		return toLong(data, 0);		
	}
	
	public static long toLong(byte [] data, int offset)
	{
		return ByteBuffer.wrap(data, offset, 8).getLong();		
	}
	
	public static double toDouble(byte [] data)
	{
		return toDouble(data, 0);		
	}
	
	public static double toDouble(byte [] data, int offset)
	{
		return ByteBuffer.wrap(data, offset, 8).getDouble();		
	}	
	
	public static boolean toBoolean(byte [] data)
	{
		return toBoolean(data, 0);		
	}
	
	public static boolean toBoolean(byte [] data, int offset)
	{
		return data[offset] == 1;
	}
}
