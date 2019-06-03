import java.io.*;
import java.util.*;
import java.lang.*;

enum token {
	END_PROGRAM,
	IDENTIFIER, LITERAL,
	ELSE, IF, WHILE,
	COMMA, SEMICOLON,
	LEFT_BRACE, RIGHT_BRACE, LEFT_PAREN, RIGHT_PAREN,
	EQUAL, OROR, ANDAND, OR, AND,
	EQEQ, NOTEQ, LE, LT, GE, GT,
	PLUS, MINUS, STAR, SLASH, PERCENT, 
	ERROR
}
public class Week1 {
	static token sy;
	static BufferedReader source;
	static int line_number;
	static char ch;
	static String id_string;
	static int literal_value;
	static void next_ch() {
		try {
			ch = (char)source.read();
			if (ch == '\n') {
				line_number++;
			}
		}
		catch(Exception e) {
			System.out.println("IO error occured");
			System.exit(1);
		}
	}
	static void get_token() {
		while(Character.isSpace(ch)) {
			next_ch();
		}												// skip whitespace
		// the cases we can identify if we know first character
		if(ch == 65535) {
			sy = token.END_PROGRAM;
			return ;
		}
		// symbol tokens
		else if(ch == '{') {
			next_ch();
			sy = token.LEFT_BRACE;
			return ;
		}
		else if(ch == '}') {
			next_ch();
			sy = token.RIGHT_BRACE;
			return ;
		}
		else if(ch == '(') {
			next_ch();
			sy = token.LEFT_PAREN;
			return ;
		}
		else if(ch == ')') {
			next_ch();
			sy = token.RIGHT_PAREN;
			return ;
		}
		else if(ch == '=') {
			next_ch();
			if(ch == '=') {
				next_ch();
				sy = token.EQEQ;
				return ;
			}
			sy = token.EQUAL;
			return ;
		}
		else if(ch == '|') {
			next_ch();
			if(ch == '|') {
				next_ch();
				sy = token.OROR;
				return ;
			}
			sy = token.OR;
			return ;
		}
		else if(ch == '&') {
			next_ch();
			if(ch == '&') {
				next_ch();
				sy = token.ANDAND;
				return ;
			}
			sy = token.AND;
			return ;
		}
		else if(ch == '!') {
			next_ch();
			if(ch == '=') {
				next_ch();
				sy = token.NOTEQ;
				return ;
			}
			// TODO : Need handle error(exclamation mark not followed by equal
			error("exclamation mark not followed by equal");
			sy = token.ERROR;
			return ;
		}
		else if(ch == '<') {
			next_ch();
			if(ch == '=') {
				next_ch();
				sy = token.LE;
				return ;
			}
			sy = token.LT;
			return ;
		}
		else if(ch == '>') {
			next_ch();
			if(ch == '=') {
				next_ch();
				sy = token.GE;
				return ;
			}
			sy = token.GT;
			return ;
		}
		else if(ch == ';') {
			next_ch();
			sy = token.SEMICOLON;
			return ;
		}
		else if(ch == ',') {
			next_ch();
			sy = token.COMMA;
			return ;
		}
		else if(ch == '+') {
			next_ch();
			sy = token.PLUS;
			return ;
		}
		else if(ch == '-') {
			next_ch();
			sy = token.MINUS;
			return ;
		}
		else if(ch == '*') {
			next_ch();
			sy = token.STAR;
			return ;
		}
		else if(ch == '%') {
			next_ch();
			sy = token.PERCENT;
			return ;
		}
		else if(ch == '/') {
			next_ch();
			if(ch != '*') {
				sy = token.SLASH;
				return ;
			}
			// TODO : need to skip comments 
			// if comment does not terminate, display error message and terminate program. 
			// NOTICE: There's no one line comment
			next_ch();
			while(ch != 65535) {
				if(ch == '*') {
					next_ch();
					if (ch == '/') { 		// comment terminated
						next_ch();
						get_token();
						return ;
					}
				}
				else {
					next_ch();
				}
			}
			line_number--;
			error("comment not terminated");
			sy = token.END_PROGRAM;
			return ;
		}
		// identify LITERAL, if there's character which is not digit, need to display error message and terminates
		else if(Character.isDigit(ch)) {
			long v = 0;
			boolean flag = false;
			while(Character.isDigit(ch)) {
				v = v * 10 + (long)(ch - '0');
				if(v > Integer.MAX_VALUE) {
					flag = true;
					break;
				}
				next_ch();
			}
			if(flag) {
				// TODO : Overflow occured, need to handle. read all following digits
				error("too large integer literal");
				while(Character.isDigit(ch)) {
					next_ch();
				}
				sy = token.LITERAL;
				literal_value = 0;
				return ;
			}
			literal_value = (int) v;
			sy = token.LITERAL;
			return ;
		}
		// identify IDENTIFIER
		else if(Character.isLetter(ch) || ch == '_') {
			id_string = "";
			while(Character.isLetter(ch) || Character.isDigit(ch) || ch == '_') {
				id_string += ch;
				next_ch();
			}
			if(id_string.equals("else")) {
				sy = token.ELSE;
				return ;
			}
			else if(id_string.equals("if")) {
				sy = token.IF;
				return ;
			}
			else if(id_string.equals("while")) {
				sy = token.WHILE;
				return ;
			}
			sy = token.IDENTIFIER;
			return ;
		}
		// unrecognized character
		else {
			// TODO : display errormessage
			next_ch();
			error("invalid character");
			sy = token.ERROR;
			return ;
		}
	}
	static void error(String s) {
		System.out.println(String.format("%4d",line_number) + ": "+s);
	}
	public static void main(String args[]) throws Exception {
		line_number = 1;
		ch = ' ';
		if(args.length == 1) {
			source = new BufferedReader(new FileReader(new File(args[0])));
		}
		else {
			source = new BufferedReader(new InputStreamReader(System.in));
			if(args.length != 0) {
				error("multiple source file is not supported");
			}
		}
		line_number = 1;
		ch = ' ';
		do{
			get_token();
			if(sy == token.IDENTIFIER) {
				System.out.println(sy + " " + id_string);
			}
			else if(sy == token.LITERAL) {
				System.out.println(sy + " " + literal_value);
			}
			else if(sy == token.ERROR) {

			}
			else {
				System.out.println(sy);
			}
		}while (sy != token.END_PROGRAM);
	}
}
