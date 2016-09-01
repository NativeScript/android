package org.nativescript.staticbindinggenerator;

public class Writer {
	private final StringBuilder sb;
	
	public Writer() {
		sb = new StringBuilder();
	}
	
	public String getSting() {
		return sb.toString();
	}
	
	public void write(char c) {
		sb.append(c);
	}
	
	public void write(int i) {
		sb.append(i);
	}
	
	public void write(boolean b) {
		sb.append(b);
	}
	
	public void write(String text) {
		sb.append(text);
	}
	
	public void writeln() {
		appendLineEnding();
	}

	public void writeln(String text) {
		write(text);
		appendLineEnding();
	}

	@Override
	public String toString() {
		return sb.toString();
	}
	
	private void appendLineEnding() {
		write("\n");
	}
}
