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

enum type { Variable, Function }

enum operation{
	LCONST, LOAD, STORE, POPUP,
	CALL, JUMP, FJUMP, TJUMP, HALT,
	MULT, DIV, MOD, ADD, SUB, ANDOP, OROP,
	EQOP, NEOP, LEOP, LTOP, GEOP, GTOP
}

class code_type{
	operation op_code;
	int operand;
}

class id_record{
	type id_class;
	int address;
	int function_id;
	int parameter_count;

	id_record(type t,int addr,int func_id,int param_count) {
		this.id_class = t;
		this.address = addr;
		this.function_id = func_id;
		this.parameter_count = param_count;
	}
	boolean isFunc() {
		return id_class == type.Function;
	}
	boolean isVar() {
		return id_class == type.Variable;
	}
}

public class Week4 {
	static token sy;
	static BufferedReader source;
	static int line_number;
	static char ch;
	static String id_string;
	static int literal_value;
	static final boolean debug_parse = false;
	static Map<String, id_record> symbol_table;
	static int variable_count;
	static final int CODE_MAX = 5000;
	static int pc = 0;
	static code_type code[] = new code_type[CODE_MAX];
	static boolean err_flag = false;
	static final int Stack_Size = 100;
	static int memory_size;
	static int sp, ic;
	static int memory[];
	static boolean trace_flag=false;;
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
		err_flag = true;
		System.out.println(String.format("%4d",line_number) + ": "+s);
	}
	public static void main(String args[]) throws Exception {
		line_number = 1;
		ch = ' ';
		init_symbol_table();
		if(args.length == 1) {
			source = new BufferedReader(new FileReader(new File(args[0])));
		}
		else {
			source = new BufferedReader(new InputStreamReader(System.in));
			if(args.length != 0) {
				error("multiple source file is not supported");
			}
		}
		String tr_str = "-t";
		if(args.length > 1 && tr_str.equals(args[1])) trace_flag = true;
		line_number = 1;
		ch = ' ';
		get_token();
		statement();
		emit(operation.HALT,0);
		if(!err_flag) {
//			print_code();
			memory_size = variable_count + Stack_Size;
			memory = new int [memory_size];
			interpret(trace_flag);
		}
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
			emit(operation.LCONST, literal_value);
			get_token();
		}
		else if( sy == token.IDENTIFIER ) {
			polish(id_string);
			String tmp = id_string;				// id_string can be modified by next get_token call
			get_token();
			id_record func;
			// function call
			int i = 0;
			if( sy == token.LEFT_PAREN ) {
				func = lookup_function(id_string);
				get_token();
				if( sy == token.RIGHT_PAREN ) {
					polish("call-0");
					if(func.parameter_count != 0) {
						error(tmp + ": number of parameters mismatch");
					}
					emit(operation.CALL, func.function_id);
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
				// identifiers is variable
				id_record var = lookup_variable(tmp);
				emit(operation.LOAD,var.address);
				return ;
			}
			if( sy == token.RIGHT_PAREN ) {
				polish("call-"+i);
				if(func.parameter_count != i) {
					error(tmp + ": number of parameters mismatch");
				}
				emit(operation.CALL,func.function_id);
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
			emit(operation.LCONST, 0);
			unary_expression();
			polish("u-");
			emit(operation.SUB, 0);
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
				emit(operation.MULT, 0);
			}
			else if( flag == 1 ) {
				polish("/");
				emit(operation.DIV, 0);
			}
			else if( flag == 2 ) {
				polish("%");
				emit(operation.MOD, 0);
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
				emit(operation.ADD, 0);
			}
			else if( flag == 1 ) {
				polish("-");
				emit(operation.SUB, 0);
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
				emit(operation.LEOP, 0);
			}
			else if( flag == 1 ) {
				polish("<");
				emit(operation.LTOP, 0);
			}
			else if( flag == 2 ) {
				polish(">=");
				emit(operation.GEOP, 0);
			}
			else if( flag == 3 ) {
				polish(">");
				emit(operation.GTOP, 0);
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
				emit(operation.EQOP, 0);
			}
			else if ( flag == 1 ) {
				polish("!=");
				emit(operation.NEOP, 0);
			}
		}
	}
	static void bit_and_expression() {
		equality_expression();
		while( sy == token.AND) {
			get_token();
			equality_expression();
			polish("&");
			emit(operation.ANDOP, 0);
		}
	}
	static void bit_or_expression() {
		bit_and_expression();
		while( sy == token.OR ) {
			get_token();
			bit_and_expression();
			polish("|");
			emit(operation.OROP, 0);
		}
	}
	static void logical_and_expression() {
		bit_or_expression();
		int prePC;
		while( sy == token.ANDAND ) {
			prePC = pc;
			emit(operation.FJUMP, 0);
			get_token();
			bit_or_expression();
			polish("&&");
			emit(operation.FJUMP, pc + 3);
			emit(operation.LCONST, 1);
			emit(operation.JUMP, pc + 2);
			emit(operation.LCONST, 0);
			code[prePC].operand = pc - 1;
		}
	}
	static void logical_or_expression() {
		logical_and_expression();
		int prePC;
		while( sy == token.OROR ) {
			prePC = pc;
			emit(operation.TJUMP, 0);
			get_token();
			logical_and_expression();
			polish("||");
			emit(operation.TJUMP, pc + 3);
			emit(operation.LCONST, 0);
			emit(operation.JUMP, pc + 2);
			emit(operation.LCONST, 1);
			code[prePC].operand = pc - 1;
		}
	}
	static void expression() {
		logical_or_expression();
		if( sy == token.EQUAL ) {
			if(code[pc-1].op_code != operation.LOAD) {
				error("assignment to non-variable");
			}
			pc--;
			int op = code[pc].operand;
			get_token();
			expression();
			polish("=");
			emit(operation.STORE,op);
		}
	}
	static void statement() {
		if( sy == token.SEMICOLON ) {
			polish("empty statement");
			if(debug_parse) System.out.println();
			get_token();
		}
		else if( sy == token.IF ) {
			int ifPC = 0,elsePC;								// javac makes error when ifPC is not initialized
			get_token();
			if( sy == token.LEFT_PAREN ) {						// right path
				boolean blankflag = false;
				get_token();
				polish("if statement:");
				if(debug_parse) System.out.println();
				if( sy == token.RIGHT_PAREN ) {
					error("need condition expression");
					blankflag = true;
				}
				if(blankflag) {
					get_token();
					if(debug_parse) System.out.println();
					statement();
				}
				else {
					expression();
					ifPC = pc;
					emit(operation.FJUMP,0);
					if( sy == token.RIGHT_PAREN) {
						get_token();
					}
					else {
						error("right parenthesis expected");
					}
					if(debug_parse) System.out.println();
					statement();
					code[ifPC].operand = pc;
				}
				if( sy == token.ELSE ) {
					polish("else part");
					if(debug_parse) System.out.println();
					elsePC = pc;
					emit(operation.JUMP,0);
					code[ifPC].operand = pc;
					get_token();
					statement();
					code[elsePC].operand = pc;
				}
				polish("end if statement");
				if(debug_parse) System.out.println();
			}
			else {
				error("left parenthesis expected after if");
			}
		}
		else if( sy == token.WHILE ) {
			get_token();
			int prePC,start;
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
					if(debug_parse) System.out.println();
					statement();
				}
				else {
					start = pc;
					expression();
					if( sy == token.RIGHT_PAREN) {
						get_token();
					}
					else {
						error("right parenthesis expected");
					}
					if(debug_parse) System.out.println();
					prePC = pc;
					emit(operation.FJUMP,0);
					statement();
					emit(operation.JUMP, start);
					code[prePC].operand = pc;
				}
				polish("end while statement");
				if(debug_parse) System.out.println();
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
			if(debug_parse) System.out.println();
			if( sy != token.SEMICOLON ) {
				error("semicolon expected");
				return ;
			}
			get_token();
			emit(operation.POPUP,0);
		}
	}	
	// Week3 Start
	static void init_symbol_table() {
		symbol_table = new TreeMap<String, id_record>();
		variable_count = 0;
		id_record x = new id_record(type.Function,-1,0,0);
		symbol_table.put("getd",x);
		x = new id_record(type.Function,-1,1,2);
		symbol_table.put("putd",x);
		x = new id_record(type.Function,-1,2,0);
		symbol_table.put("newline",x);
		x = new id_record(type.Function,-1,3,1);
		symbol_table.put("putchar",x);
	}
	static id_record search(String name) {
		if(symbol_table.get(name) != null) {
			return symbol_table.get(name);
		}
		else {
			id_record x = new id_record(type.Variable, variable_count, -1, -1);
			variable_count++;
			symbol_table.put(name,x);
			return x;
		}
	}
	static id_record lookup_variable(String name) {
		id_record x = search(name);
		if(x.isFunc()) {
			error(name + ": function is used as variable");
		}
		return x;
	}
	static id_record lookup_function(String name) {
		id_record x = search(name);
		if(x.isVar()) {
			error(name + ": variable is used as function");
		}
		return x;
	}
	static void emit(operation op, int param) {
		if(pc >= CODE_MAX) {
			error("Code overflow");
			System.exit(-1);
		}
		code[pc] = new code_type();
		code[pc].op_code = op;
		code[pc].operand = param;
		pc++;
	}
	static void print_code() {
		for (int i=0;i<pc;i++) {
			System.out.println(String.format("%5d",i)+": "+
							   String.format("%-6s",code[i].op_code) +
							   String.format("%6d",code[i].operand));
		}
	}
	// week4 
	static void run_error(String s) {
		System.out.println(s);
		System.exit(1);
	}
	static void push(int x) {
		if(sp >= memory_size)
			run_error("stack overflow");
		memory[sp] = x;
		sp++;
	}
	static int pop() {
		if(sp <= variable_count)
			run_error("system error: stack underflow");
		sp--;
		return (memory[sp]);
	}
	static void interpret(boolean trace) {
		operation instruction;
		int argument;
		int left, right, tmp;
		Scanner sc = new Scanner(System.in);
		ic = 0;
		sp = variable_count;
		while(true) {
			instruction = code[ic].op_code;
			argument = code[ic].operand;
			if(trace) {
				System.out.print("ic="+String.format("%4d",ic)+
						", sp="+String.format("%5d",sp)+
						", code=("+
						String.format("%-6s", instruction) +
						String.format("%6d", argument)+")");
				if(sp > variable_count) {
					int val = pop();
					push(val);
					System.out.print(", top="+String.format("%10d",val));
				}
				System.out.println();
			}
			ic++;
			switch(instruction) {
				case CALL:
					if(argument == 0) {	// getd
						System.out.print("getd: ");
						push(sc.nextInt());
					}
					else if(argument == 1) {	// putd
						int width = pop();
						int val = pop();
						String s = String.format("%d",val);
						int d = width-s.length();
						while(d>0) {
							System.out.print(" ");
							d--;
						}
						System.out.print(s);
						push(val);
					}
					else if(argument == 2) {	// newline
						System.out.println();
						push(0);
					}
					else if(argument == 3) {	// putchar
						int val = pop();
						char c = (char) val;
						System.out.print(c);
						push(val);
					}
					else {
						run_error("Undefined Function!");
					}
					break;
				case LCONST:
					push(argument);
					break;
				case LOAD:
					if(argument >= variable_count || argument < 0) run_error("Undefined Variable");
					push(memory[argument]);
					break;
				case STORE:
					if(argument >= variable_count || argument < 0) run_error("Undefined Variable");
					tmp = pop();
					memory[argument] = tmp;
					push(tmp);
					break;
				case POPUP:
					pop();
					break;
				case JUMP:
					ic = argument;
					break;
				case FJUMP:
					tmp = pop();
					if(tmp == 0) ic = argument;
					break;
				case TJUMP:
					tmp = pop();
					if(tmp != 0) ic = argument;
					break;
				case HALT:
					if( sp != variable_count ) run_error("Stack Corrupted");
					return ;
				case MULT:
					right = pop();
					left = pop();
					push(left * right);
					break;
				case ADD:
					right = pop();
					left = pop();
					push(left + right);
					break;
				case SUB:
					right = pop();
					left = pop();
					push(left - right);
					break;
				case ANDOP:
					right = pop();
					left = pop();
					push(left & right);
					break;
				case OROP:
					right = pop();
					left = pop();
					push(left | right);
					break;
				case DIV:
					right = pop();
					left = pop();
					if(right == 0) run_error("Division by 0");
					push(left / right);
					break;
				case MOD:
					right = pop();
					left = pop();
					if(right == 0) run_error("Modulus by 0");
					push(left % right);
					break;
				case EQOP:
					right = pop();
					left = pop();
					if(left == right) push(1);
					else push(0);
					break;
				case NEOP:
					right = pop();
					left = pop();
					if(left != right) push(1);
					else push(0);
					break;
				case LEOP:
					right = pop();
					left = pop();
					if(left <= right) push(1);
					else push(0);
					break;
				case LTOP:
					right = pop();
					left = pop();
					if(left < right) push(1);
					else push(0);
					break;
				case GEOP:
					right = pop();
					left = pop();
					if(left >= right) push(1);
					else push(0);
					break;
				case GTOP:
					right = pop();
					left = pop();
					if(left > right) push(1);
					else push(0);
					break;
				default:
					run_error("system error: undefined op code");
					break;
			}
		}
	}
}
