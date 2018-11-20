package org.csystem.client;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Scanner;

import org.csystem.net.SocketUtil;
import org.csystem.util.ByteConverter;

public class Client {
	
	public void run()
	{		
		try (Scanner kb = new Scanner(System.in); Socket socket = new Socket("172.20.29.186", 5120)) {
			InputStream is = socket.getInputStream();
			OutputStream os = socket.getOutputStream();
			
			DataOutputStream dos = new DataOutputStream(os);
			DataInputStream dis = new DataInputStream(is);
			
			System.out.println(os.getClass().getName());
					
			for (;;) {
				System.out.print("Text:");
				String text = kb.nextLine();
				
				byte [] buf = text.getBytes();
				byte [] bufLen = ByteConverter.getBytes(buf.length);
				
				int written = SocketUtil.write(dos, bufLen);
				System.out.println("Written:" + written);
				
				written = SocketUtil.write(dos, buf);
				System.out.println("Written:" + written);
				
				if (text.equals("quit"))
					break;
				
				bufLen = new byte[4];
				int read = SocketUtil.read(dis, bufLen);
				System.out.println("Read:" + read);
				
				int len = ByteConverter.toInt(bufLen);
				
				System.out.printf("Len=%d%n", len);
				
				buf = new byte[len];
				
				read = SocketUtil.read(dis, buf);
				
				System.out.println("Read:" + read);
				
				System.out.println(new String(buf));				
			}
		}
		catch (Throwable ex) {
			ex.printStackTrace();
		}	
		
	}

}
