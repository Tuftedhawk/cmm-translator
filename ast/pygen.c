#include <stdio.h>
#include "ast.h"

void traverse(AST node) {
	if (!node) return;
	node_count++;

	switch (node->kind) {
		case AST_INT:
			printf("%d",node->ival);
			break;

		case AST_BINOP:
			const char* PythonBONames[] {
				"**",
				"+",
				"-",
				"*",
				"/",
				"%",
				"<",
				"<=",
				">",
				">=",
				"==",
				"!=",
				"and",
				"or",
				"="
			};
			putchar('(');
			traverse(node->binop.a);
			printf(" %s ", PythonBONames[node->binop.op]);
			traverse(node->binop.b);
			putchar(')');
			break;

		case AST_IF:



		case AST_WHILE:


		case AST_RETURN:


		case AST_VAR_DECL:


		case AST_VD_ITEM:


		case AST_FUNDEF:


		case ALIST_STMTS:


	}
}
