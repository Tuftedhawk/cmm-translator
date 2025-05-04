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

static const char* PythonUONames[] ={
                                "-",
                                "not ",
                                "", //prepp ++x
                                "", //postpp x++
                                "", //premm --x
                                "" //postmm x--
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

                case AST_UNOP://covers unary operators
                        switch (node->unop.op) {
                                case UOP_UMINUS:
                                case UOP_NOT:
                                printf("%s", PythonUONames[node->unop.op]);
                                traverse_py(node->unop.a);
                                break;

                                case UOP_PREPP:
                                traverse_py(node->unop.a);
                                printf(" += 1");
                                break;

                                case UOP_POSTPP:
                                traverse_py(node->unop.a);
                                printf(" += 1");
                                break;

                                case UOP_PREMM:
                                traverse_py(node->unop.a);
                                printf(" -= 1");
                                break;

                                case UOP_POSTMM:


                                traverse_py(node->unop.a);
                                printf(" -= 1");
                                break;
                                }
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
		if (node->fundef.name){ //if statements to prevent seg faults for NULL cases
			printf("def %s(", node->fundef.name);
		}
		else{
			printf("def func("); //if the name is no provided
		}
		if (node->fundef.params) {
			traverse_py(node->fundef.params);
		}
		printf("):\n"); // def func( parms ); <- closes
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
                case ALIST_PARAMS:                            // this will iterate the list of parameters but
		for (int i = 0; i < node->list.length; i++) { //AST_PARAMS has the references
			if (i > 0) printf(", ");              // not the names just the structure
			if (node->list.nodes[i]) {			
				traverse_py(node->list.nodes[i]);
			} 
			else {
				printf(" ");//NULL
			}
		}
		break;
	       
	       	case AST_PARAM:// this will print the name of each parm
                if (node->param.name) { // and its seperate to prevent a seg fault
                        printf("%s", node->param.name);
                }
                else {
                        printf(" "); //no name for parameter
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
