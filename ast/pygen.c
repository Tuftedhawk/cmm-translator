#include <stdio.h>
#include "ast.h"

static int indent_level = 0;

static const char* PythonBONames[] ={
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

void print_indent() {
	for (int i = 0; i < indent_level; i++){
		printf("    ");
	}
}

void traverse_py(AST node) {;
	if (node == NULL) {
		return;
	}
	
	switch (node->kind) {
		case AST_INT:
			printf("%d",node->ival);
			break;

		case AST_BINOP:
			traverse_py(node->binop.a);
			printf(" %s ", PythonBONames[node->binop.op]);
			traverse_py(node->binop.b);
			break;

		case AST_IF:
			print_indent();
			printf("if ");
			traverse_py(node->if_stmt.cond);
			printf(":\n");
			indent_level++;
			traverse_py(node->if_stmt.then_branch);
			indent_level--;
			if (node->if_stmt.else_branch) {
				print_indent();
				printf("else:\n");
				indent_level++;
				traverse_py(node->if_stmt.else_branch);
				indent_level--;
			}
			break;

		case AST_WHILE:
			print_indent();
			printf("while ");
			traverse_py(node->while_stmt.cond);
			printf(":\n");
			indent_level++;
			traverse_py(node->while_stmt.body);
			indent_level--;
			break;
		
		case AST_RETURN:
			print_indent();
			printf("return ");
			traverse_py(node->ast);
			printf("\n");
			break;

		case AST_VAR_DECL:
			for (int i = 0; i < node->vdecl.vars->list.length; i++) {
				print_indent();
				traverse_py(node->vdecl.vars->list.nodes[i]);
				printf("\n");
			}
			break;

		case AST_VD_ITEM:
			printf("%s", node->vd_item.name);
			if (node->vd_item.init) {
				printf(" = ");
				traverse_py(node->vd_item.init);
			}
			break;

		case AST_FUNDEF:
			print_indent();
			printf("def %s(", node->fundef.name);
			traverse_py(node->fundef.params);
			printf("):\n");
			indent_level++;
			traverse_py(node->fundef.body);
			indent_level--;
			break;

		case ALIST_STMTS:
			if (node->list.nodes) {
				for (int i = 0; i < node->list.length; i++) {
					traverse_py(node->list.nodes[i]);
				}
			}
			break;

        	case AST_SCALAR:
    			printf("%s", node->name);
    			break;

    		case AST_BOOL:
        		if (node->ival)
            			printf("True");
        		else
            			printf("False");
        		break;

    		case AST_AL:
        		printf("len(%s)", node->name);
       			break;

    		case AST_AE:
        		printf("%s[", node->ae.array);
        		traverse_py(node->ae.index);
        		printf("]");
        		break;

    		case AST_ES:
        		print_indent();
        		traverse_py(node->ast);
        		printf("\n");
        		break;

    		case ALIST_ARGS:
    		case ALIST_PARAMS:
        		if (node->list.nodes && node->list.length > 0) {
            			traverse_py(node->list.nodes[0]);
            			for (int i = 1; i < node->list.length; i++) {
                			printf(", ");
                			traverse_py(node->list.nodes[i]);
            			}
        		}
        		break;
            	case AST_FUNCALL:
    			printf("%s(", node->funcall.name);
    			traverse_py(node->funcall.args);
    			printf(")");
    			break;

		case ALIST_TOP_DECL:
		      if (node->list.nodes) {
			      for (int i = 0; i < node->list.length; i++) {
				      traverse_py(node->list.nodes[i]);
			      }
		      }
		      break;

		default:
			fprintf(stderr, "Don't know how to process %d\n", node->kind);
			break;
	}
}
void pygen(AST root) {
	traverse_py(root);
}
