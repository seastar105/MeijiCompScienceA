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
public class Week2 {
	static token sy;
	static BufferedReader source;
	static int line_number;
	static char ch;
	static String id_string;
	static int literal_value;
	static final boolean debug_parse = true;
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
		get_token();
		statement();
//		expression();
		if(sy != token.END_PROGRAM) {
			error("extra text at the end of the program");
		}
	}
	static void polish(String s) {
		if (debug_parse) {
			System.out.print(s+" ");
		}
	}
	// statement process
	// check ';' , expression , if , while , complex(parenthesis) using if-else
	// follow this order after call expression
	// expression => logic-or => logic-and => bit-or => bit-and => logic-eq =>
	// relation => additive => multiplicative => unary => primary
	static void primary_expression() {
		if( sy == token.LITERAL ) {
			polish(literal_value+"");
			get_token();
		}
		else if( sy == token.IDENTIFIER ) {
			polish(id_string);
			get_token();
			// function call
			int i = 0;
			if( sy == token.LEFT_PAREN ) {
				get_token();
				if( sy == token.RIGHT_PAREN ) {
					polish("call-0");
					get_token();
					return ;
				}
				expression();
				i++;
				while( sy == token.COMMA ) {
					get_token();
					expression();
					i++;
				}
			}
			else {
				return ;
			}
			if( sy == token.RIGHT_PAREN ) {
				polish("call-"+i);
				get_token();
			}
			else {
				error("right parenthesis expected");
			}
		}
		else if( sy == token.LEFT_PAREN ) {
			get_token();
			expression();
			if( sy == token.RIGHT_PAREN ) {
				get_token();
			}
			else {
				error("right parenthesis expected");
			}
		}
		else {
			error("unrecognized element in expression");
			get_token();
		}
	}
	static void unary_expression() {
		if( sy == token.MINUS ) {
			get_token();
			unary_expression();
			polish("u-");
		}
		else {
			primary_expression();
		}
	}
	static void multiplicative_expression() {
		int flag = -1;
		unary_expression();
		while( sy == token.STAR || sy == token.SLASH || sy == token.PERCENT) {
			if( sy == token.STAR ) {
				flag = 0;
			}
			else if( sy == token.SLASH ) {
				flag = 1;
			}
			else if( sy == token.PERCENT ) {
				flag = 2;
			}
			get_token();
			unary_expression();
			if( flag == 0 ) {
				polish("*");
			}
			else if( flag == 1 ) {
				polish("/");
			}
			else if( flag == 2 ) {
				polish("%");
			}
			else {
				error("something wrong");
			}
		}
	}
	static void additive_expression() {
		int flag = -1;
		multiplicative_expression();
		while( sy == token.PLUS || sy == token.MINUS ) {
			if( sy == token.PLUS ) {
				flag = 0;
			}
			else if( sy == token.MINUS ) {
				flag = 1;
			}
			get_token();
			multiplicative_expression();
			if( flag == 0 ) {
				polish("+");
			}
			else if( flag == 1 ) {
				polish("-");
			}
			else {
				error("something wrong");
			}
		}
	}
	static void relational_expression() {
		int flag = -1;
		additive_expression();
		while( sy == token.LE || sy == token.LT
				|| sy == token.GE || sy == token.GT ) {
			if( sy == token.LE ) flag = 0;
			else if( sy == token.LT ) flag = 1;
			else if( sy == token.GE ) flag = 2;
			else if( sy == token.GT ) flag = 3;
			get_token();
			additive_expression();
			if( flag == 0 ) {
				polish("<=");
			}
			else if( flag == 1 ) {
				polish("<");
			}
			else if( flag == 2 ) {
				polish(">=");
			}
			else if( flag == 3 ) {
				polish(">");
			}
			else {
				error("something wrong");
			}
		}
	}
	static void equality_expression() {
		int flag = -1;
		relational_expression();
		while( sy == token.EQEQ || sy == token.NOTEQ ) {
			if( sy == token.EQEQ ) flag = 0;
			else if( sy == token.NOTEQ ) flag = 1;
			get_token();
			relational_expression();
			if( flag == 0 ) {
				polish("==");
			}
			else if ( flag == 1 ) {
				polish("!=");
			}
		}
	}
	static void bit_and_expression() {
		equality_expression();
		while( sy == token.AND) {
			get_token();
			equality_expression();
			polish("&");
		}
	}
	static void bit_or_expression() {
		bit_and_expression();
		while( sy == token.OR ) {
			get_token();
			bit_and_expression();
			polish("|");
		}
	}
	static void logical_and_expression() {
		bit_or_expression();
		while( sy == token.ANDAND ) {
			get_token();
			bit_or_expression();
			polish("&&");
		}
	}
	static void logical_or_expression() {
		logical_and_expression();
		while( sy == token.OROR ) {
			get_token();
			logical_and_expression();
			polish("||");
		}
	}
	static void expression() {
		logical_or_expression();
		if( sy == token.EQUAL ) {
			get_token();
			expression();
			polish("=");
		}
	}
	static void statement() {
		if( sy == token.SEMICOLON ) {
			System.out.println("empty statement");
			get_token();
		}
		else if( sy == token.IF ) {
			get_token();
			if( sy == token.LEFT_PAREN ) {						// right path
				boolean blankflag = false;
				get_token();
				polish("if statement:");
				if( sy == token.RIGHT_PAREN ) {
					error("need condition expression");
					blankflag = true;
				}
				if(blankflag) {
					get_token();
					System.out.println();
					statement();
				}
				else {
					expression();
					if( sy == token.RIGHT_PAREN) {
						get_token();
					}
					else {
						error("right parenthesis expected");
					}
					System.out.println();
					statement();
				}
				if( sy == token.ELSE ) {
					System.out.println("else part");
					get_token();
					statement();
				}
				System.out.println("end if statement");
			}
			else {
				error("left parenthesis expected after if");
			}
		}
		else if( sy == token.WHILE ) {
			get_token();
			if( sy == token.LEFT_PAREN ) {
				boolean blankflag = false;
				get_token();
				polish("while statement:");
				if( sy == token.RIGHT_PAREN ) {
					error("need condition expression");
					blankflag = true;
				}
				if(blankflag) {
					get_token();
					System.out.println();
					statement();
				}
				else {
					expression();
					if( sy == token.RIGHT_PAREN) {
						get_token();
					}
					else {
						error("right parenthesis expected");
					}
					System.out.println();
					statement();
				}
				System.out.println("end while statement");
			}
			else {
				error("left parenthesis expected after while");
			}
		}
		else if( sy == token.LEFT_BRACE ) {
			get_token();
			if( sy == token.RIGHT_BRACE) { 		// empty brace
				get_token();
				return ;
			}
			while(true) {
				statement();
				if( sy == token.RIGHT_BRACE ) {
					get_token();
					return ;
				}
				if( sy == token.END_PROGRAM ) {
					error("too few right braces at end of statement list");
					sy = token.ERROR;
					return ;
				}
			}
		}
		else {
			expression();
			System.out.println();
			if( sy != token.SEMICOLON ) {
				error("semicolon expected");
			}
			get_token();
		}
	}
}
