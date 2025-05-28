#ifndef BIC
    #include "bic.h"
#endif

// checks if token is the given symbol
#define eq_sym(tok, sym, side)\
(tok->type == LSYMBOL and tok->vall.num == sym and tok->apdx == side)

// checks if token is the given operator
#define eq_opr(tok, i_idx, f_idx)\
(tok->type == OPERATOR and tok->vall.num >= i_idx and tok->vall.num <= f_idx)

#define eq_opr_range(tok, t)\
(tok->type == OPERATOR and tok->vall.num >= t[0] and tok->vall.num <= t[1])

#define eq_kwd(tok, k)\
(tok->type == KEYWORD and tok->vall.num == k)

#define eq_kwd_range(tok, t)\
(tok->type == KEYWORD and tok->vall.num >= t[0] and tok->vall.num <= t[1])

// appends a token at the end of the path as a node
#define push_node(n, t){\
    n->next = alloc(sizeof(node));\
    n->next->last = n;\
    n->next->type = t->type;\
    n->next->vall = t;\
    n = n->next;\
}

// code path rules

// variable namespace definition codepath's rule
node *define_r(tkn *c){
    node *path = alloc(sizeof(node));
    node *pntr = alloc(sizeof(node));

    // basic metadata
    path->is_parent = T;
    path->type = DEFINE;
    path->ftok = c;
    path->stt  = pntr;

    // start branches
    pntr->type = KEYWORD;
    pntr->vall = c;

    // definition has attribution
    if(c->next->next->type == OPERATOR){
        if(!eq_opr(c->next->next, 0, 0))
            cmperr(DEFWOEQ, c->next->next, nil);

        // a definition is an keyword and an assignment
        // so evaluate the last part and append the kwd
        pntr->next = assign_r(c->next->next, F);
        pntr->next->last = pntr;
        pntr = pntr->next;

        path->ltok = pntr->ltok;
        path->end  = pntr;

    // only namespace
    } else if(c->next->type == INDEXER){
        path->ltok = c->next;
        path->end  = pntr;
        push_node(pntr, c->next);

    // handle errors
    } else if(c->next->type == LITERAL)
        cmperr(LITRIDX, c->next, nil);

    else if(c->next->type == KEYWORD)
        cmperr(KWRDIDX, c->next, nil);

    else cmperr(INVALID, c->next, nil);

    path->end->next = path->stt;
    path->stt->last = path->end;
    return path;
}

// assignment codepath's rule
node *assign_r(tkn *c, bool prnd){
    // validate operator
    if(!eq_opr_range(c, asgn))
        cmperr(NOTASGN, c, nil);

    node *path = alloc(sizeof(node));

    path->is_parent = T;
    path->type = ASSIGN;

    tkn *strt = c->last, *prox = c->next->next;

    // Type A: <type> foo = bar [, spm = egg]*

    // validate the left-hand
    node *pntr = validthnd(strt, T, F);
    path->stt = pntr;

    if(pntr == nil) cmperr(INVALID, strt, nil);

    // now push the operator itself
    push_node(pntr, c);

    // validate the right-hand
    if(c->next->type == INDEXER or c->next->type == LITERAL){
        // check for expressions
        if(prox->type == OPERATOR){
            // invalid operators
            if(prox->vall.num == 29 or prox->vall.num == 31)
                cmperr(UNEXPCT, prox, nil);

            pntr->next = exprss_r(prox, F);
            pntr->next->last = pntr;
            pntr = pntr->next;
            prox = pntr->ltok->next;
        // another sentence
        } else {
            // push righthand
            pntr->next = validthnd(c->next, F, F);
            pntr->next->last = pntr;
            pntr = pntr->next;

            if(pntr == nil) cmperr(EXPVRHD, c->next, nil);

            // foo = bar + buzz, egg = bar + spam, ...
            // ................^
            //                 you are here

            prox = pntr->ltok->next;
        }
    // the righthand is an expression
    } else if(eq_sym(c->next, SYM_PAR, 0)){
        // an encapsulated signed value
        if(prox->type == OPERATOR){
            pntr->next = exprss_r(prox, T);
            pntr->next->last = pntr;
            pntr = pntr->next;

        // it's really an expression
        } else if(prox->next->type == OPERATOR){
            pntr->next = exprss_r(prox->next, T);
            pntr->next->last = pntr;
            pntr = pntr->next;

        // just a value surrounded by parentheses
        } else if(prox->type == LITERAL
            or prox->type == INDEXER
            or prox->type == KEYWORD){

            pntr->next = validthnd(prox, F, F);
            pntr->next->last = pntr;
            pntr = pntr->next;

            if(pntr == nil) cmperr(EXPVRHD, prox, nil);

            // after node-pushing it, the
            // pntr's last token field is
            // null (other cases aren't).
            pntr->ltok = prox;
        }
        prox = pntr->ltok;
    // syntax error
    } else cmperr(UNEXPCT, c->next, nil);

    // check for multiple definitions
    while(eq_sym(prox, SYM_COM, 0)){
        // evaluate next assignation
        pntr->next = assign_r(prox, F);

        // append it
        pntr->next->last = pntr;
        pntr = pntr->next;
        prox = pntr->ltok->next;
    }

    if(prnd){
        if(eq_sym(prox, SYM_PAR, 1)){
            push_node(pntr, prox);
            prox = prox->next;
        } else cmperr(EXPCTDP, prox, nil);
    }

    // TODO: handle multiple assignment

    path->ltok = prox->last;
    path->end  = pntr;

    path->end->next = path->stt;
    path->stt->last = path->end;

    assert(path->stt == pntr->next, nil);
    return path;
}
// structure definition codepath's rule
// the given token must be the first indexer
node *strdef_r(tkn *c){
    assert(c->type == INDEXER, nil);
    assert(eq_sym(c->next, SYM_BRA, 0), nil);

    node *path = alloc(sizeof(node));
    node *pntr = alloc(sizeof(node));

    path->stt  = pntr;
    path->ftok = c;
    path->type = STRDEF;
    path->is_parent = T;

    pntr->type = KEYWORD;
    pntr->vall = c;

    // match end of expression
    path->ltok = matchpair(c->next);
    if(path->ltok == c->next->next)
        return path;

    // validate every field assignment
    for(tkn *p = c->next->next; p != path->ltok; p = p->next){
        node *fld = validthnd(p, F, F);
        // the field value is an expression
        if(eq_kwd_range(fld->ltok->next, asgn)){
            tkn *temp = fld->ltok->next;
            free(fld);

            pntr->next = exprss_r(temp, F);
            pntr->next->last = pntr;
            pntr = pntr->next;

        // it's only one value
        } else {
            pntr->next = fld;
            pntr->next->last = pntr;
            pntr = pntr->next;
            pntr->ltok = p;
        }
    }
    
    path->ltok = pntr->ltok;
    path->end  = pntr;

    path->end->next = path->stt;
    path->stt->last = path->end;

    return path;
}
// enum definition codepath's rule
node *enumdf_r(tkn *c){
    node *path = nil;
    assert(F, "enums not implemented yet");
    return path;
}
// structure literal codepath's rule
node *struct_r(tkn *c){
    assert(eq_kwd(c, KW_STRUCT), nil);
    assert(eq_sym(c->next, SYM_BRA, 0), nil);

    node *path = alloc(sizeof(node));
    node *pntr = alloc(sizeof(node));

    path->ftok = c;
    path->stt  = pntr;

    pntr->type = INDEXER;
    pntr->vall = c;

    // match end of expression
    path->ltok = matchpair(c->next);

    // no fields defined
    if(path->ltok == c->next->next)
        cmperr(EMPTYIM, c->next->next, nil);
    
    // validate every field
    for(tkn *p = c->next->next; p != path->ltok; p = p->next){
        // check if syntax matches
        if(eq_kwd_range(p, ldef)){
            pntr->next = define_r(p);
            pntr->next->last = pntr;
            pntr = pntr->next;
            
            // move to the end of the path
            p = pntr->ltok;

        // not a field definition
        } else cmperr(UNEXPCT, p, nil);
    }
    path->end = pntr;
    path->ltok = pntr->ltok;

    path->end->next = path->stt;
    path->stt->last = path->end;

    return path;
}
// constant definition codepath's rule
node *constd_r(tkn *c){
    node *path = alloc(sizeof(node));
    node *pntr = alloc(sizeof(node));

    path->is_parent = T;
    path->type = CONSTD;
    path->ftok = c;
    path->stt  = pntr;

    pntr->type = INDEXER;
    pntr->vall = c;

    // the sentense is fallowing the path `NAMESPACE ( ... );`
    if(eq_sym(c->next, SYM_PAR, 0)){
        tkn *end = matchpair(c->next);

        // it's a function
        if(eq_sym(end->next, SYM_BRA, 0)
        or end->next->type == INDEXER
        or end->next->type == LITERAL){
            // do not be redundant
            free(path);
            free(pntr);
            return fun_def_r(c->next);

        // a funcall
        } else if(eq_sym(end->next, SYM_CLN, 0)){
            free(path);
            free(pntr);
            return funcall_r(c);

        // invalid syntax
        } else cmperr(UNEXPCT, end->next, nil);

    // an array definition
    } else if(eq_sym(c->next, SYM_BRA, 0)){
        free(path);
        free(pntr);
        return arrdef_r(c->next);

    // expression
    } else if(c->next->next->type == OPERATOR){
        // validade path
        pntr->next = exprss_r(c->next->next, F);
        pntr->next->last = pntr;
        pntr = pntr->next;

        // finish with metadata
        path->ltok = pntr->ltok;
        pntr->end  = pntr;

        path->end->next = path->stt;
        path->stt->last = path->end;
        return path;

    // value
    } else if(c->next->type == INDEXER or c->next->type == LITERAL){
        push_node(pntr, c->next);
        
        // finish with metadata
        path->ltok = c->next;
        pntr->end  = pntr;

        path->end->next = path->stt;
        path->stt->last = path->end;
        return path;

    // invalid token
    } else {
        if(c->next->type == KEYWORD) cmperr(KWRDVAL, c->next, nil);
        else cmperr(UNEXPCT, c->next, nil);
    }
    return nil;
}
// arithmetic and boolean expressions codepath's rule
node *exprss_r(tkn *c, bool prnd){
    // redirect if it's an assignment
    if(eq_opr_range(c, asgn) and c->apdx == 0)
        return assign_r(c, prnd);
    else if(c->type != OPERATOR)
        cmperr(EXPCTEX, c->last, nil);

    node *path = alloc(sizeof(node));
    node *pntr;

    path->is_parent = T;
    path->type = EXPRSS;

    // it's a binary operator
    if(c->last->type == INDEXER
    or c->last->type == LITERAL
    or eq_sym(c->last, SYM_PAR, 1)){
        // append lefthand
        pntr = validthnd(c->last, F, F);
        if(pntr == nil) cmperr(UNEXPCT, c->last, nil);

        path->stt  = pntr;

        // append operator
        push_node(pntr, c);

        // both sides are valid members
        if(c->next->type == INDEXER
        or c->next->type == LITERAL){
            // this is not the entire expression
            if(c->next->next->type == OPERATOR){
                pntr->next = exprss_r(c->next->next, prnd);
                pntr->next->last = pntr;
                pntr = pntr->next;
                path->ltok = pntr->ltok;

            // simple expression
            } else {
                push_node(pntr, c->next);
                path->ltok = c->next;
            }
        // so is righthand invalid?
        } else {
            // the current path fallows the `foo <opr> (bar[ ...])` syntax
            if(eq_sym(c->next, SYM_PAR, 0)){
                // c->next->next => <opr> >> ( >> <lhd>
                tkn *rhnd = c->next->next;

                // righthand is a `(<opr> foo)` path
                if(rhnd->type == OPERATOR){
                    pntr->next = exprss_r(rhnd, T);
                    pntr->next->last = pntr;
                    pntr = pntr->next;
                    path->ltok = pntr->ltok;

                // it's really an expression
                } else if(rhnd->next->type == OPERATOR){
                    pntr->next = exprss_r(rhnd->next, T);
                    pntr->next->last = pntr;
                    pntr = pntr->next;
                    path->ltok = pntr->ltok;

                // just a value surrounded by parentheses
                } else if(rhnd->type == INDEXER or rhnd->type == LITERAL){
                    push_node(pntr, rhnd);
                    path->ltok = rhnd;
                }
            // function-like keywords
            } else if(eq_kwd_range(c->next, funl)){
                pntr->next = funcall_r(c->next);
                pntr->next->last = pntr;
                pntr = pntr->next;

                path->ltok = pntr->ltok;
            // errors
            } else {
                if(c->apdx == 0){
                    // it's an assignment operator
                    if(eq_opr_range(c, asgn))
                        cmperr("invalid value for assignment", c->next, nil);
                    // something else
                    else if(eq_opr_range(c, eqlt))
                        cmperr("righthand cannot be evaluated", c->next, nil);
                    else cmperr(UNEXPCT, c->next, nil);
                } else cmperr(UNEXPCT, c->next, nil);
            }
        }
    // unary
    } else if(eq_opr_range(c, unry)){

        // append the operator
        pntr = alloc(sizeof(node));
        pntr->type = OPERATOR;
        pntr->vall = c;

        cmperr("HERE!", c->next, nil);

        pntr->next = validthnd(c->next, F, F);
        pntr->next->last = pntr;
        pntr = pntr->next;
        if(pntr == nil) cmperr(EXPVHND, c->next, nil);

        tkn *after = c->next->next;

        // move to the end of the current path
        if(pntr->ltok) after = pntr->ltok;

        // it's an expression
        if(after->next->type == OPERATOR){
            node *temp = pntr->last;
            free(pntr);

            temp->next = exprss_r(after->next, F);
            pntr = temp->next;
        }

    } else cmperr(
        "lefthand of the expression"
        " is missing or invalid", c->next, nil);

    // continue evaluating expression
    if(path->ltok->next->type == OPERATOR){
        // avoid duplication
        pntr = pntr->last;
        free_node(pntr->next, T);

        pntr->next = exprss_r(path->ltok->next, prnd);
        pntr->next->last = pntr;
        pntr = pntr->next;

        path->end  = pntr;
        path->ltok = pntr->ltok;
    }

    // last syntax check
    if(prnd){
        // move ltok pointer forward
        if(eq_sym(path->ltok->next, SYM_PAR, 1))
            path->ltok = path->ltok->next;
        // syntax error
        else cmperr(EXPCTDP, path->ltok->next, nil);
    }

    // finish with metadata
    path->end = pntr;

    path->end->next = path->stt;
    path->stt->last = path->end;
    return path;
}
// array definition codepath rule
node *arrdef_r(tkn *c){
    node *path = nil;
    assert(F, "arrays not implemented yet");
    return path;
}
// statement declaration codepath rule
node *sttmnt_r(tkn *c){
    node *path = alloc(sizeof(node));
    node *pntr = alloc(sizeof(node));

    // basic metadata
    path->is_parent = T;
    path->type = STTMNT;
    path->ftok = c;
    path->stt  = pntr;

    // start branches
    pntr->type = KEYWORD;
    pntr->vall = c;

    /* there are only three types of statements *\
     *                                           *
     * + function-like                           *
     * like `sizeof` and `typeof`                *
     *                                           *
     * + single                                  *
     * like `break` and `next`                   *
     *                                           *
     * + expression holders                      *
     * like `return` and `goto`                  *
     *                                           *
     * + body holder                             *
     * the only one (for now) is `else`          *
     *                                           *
     * + true statements                         *
     * like `if` and `for`                       *
     *                                           *
     * so by using this logic of classifications *
     * you can check the syntax of multiple sta- *
    \* -tements with fewer cases.               */

    // true statements
    if(eq_kwd_range(c, trus)){
        bool jmpd = F, inpar = F;
        tkn *innr = c->next;
        // just jump over parentheses for now
        if((inpar = eq_sym(innr, SYM_PAR, 0))) innr = innr->next;

    run_again:
        // main expression of the statement (keyword [exp] {};)
        if(innr->type == INDEXER or innr->type == LITERAL){

            // whole expression
            if(innr->next->type == OPERATOR){
                pntr->next = exprss_r(innr->next, F);

                pntr->next->last = pntr;
                pntr = pntr->next;

                innr = pntr->ltok->next;
            // single value
            } else {
                pntr->next = validthnd(innr, F, F);
                if(pntr->next == nil) cmperr(EXPVHND, innr, nil);
                
                pntr->next->last = pntr;
                pntr = pntr->next;

                innr = pntr->ltok->next;
            }

            // last expression of the loop
            if(eq_kwd(c, KW_FOR) and
            eq_sym(pntr->ltok->next, SYM_CLN, 0)){
                tkn* stt = pntr->ltok->next->next;
                if(stt->next->type == OPERATOR){
                    // handle expression
                    pntr->next = exprss_r(stt->next, F);
                    pntr->next->last = pntr;
                    pntr = pntr->next;
                // expression not found
                } else cmperr(UNEXPCT, stt->next, &(tkn){
                    .vall.str = FORSNTX,
                    .apdx = 1
                });
            }

            tkn *body_s = pntr->ltok->next;
            if(eq_sym(body_s, SYM_PAR, 1)) body_s = body_s->next;

            // TODO: handle for loop mode 2

            // it must be the end of the statement head
            if(eq_sym(body_s, SYM_BRA, 0)){
                pntr->next = parse(body_s->next, SCOPE);

                pntr->next->last = pntr;
                pntr = pntr->next;
                path->ltok = pntr->ltok;

            } else cmperr(EXPCTBD, body_s,
                &(tkn){.vall.str = OBS_IFB, .apdx = 1});

        // statement definition block (keyword [asgn]; exp {};)
        } else if(eq_kwd_range(innr, ldef)){
            // only a single definition block is allowed
            if(!jmpd){
                node *defn = define_r(innr);

                pntr->next = defn->stt;
                pntr->next->last = pntr;
                pntr = pntr->next;

                // move pointer to the end of the path
                if(eq_sym(defn->ltok->next, SYM_CLN, 0)){
                    innr = defn->ltok->next->next;
                // expected semicolon
                } else cmperr(NOTERMN, defn->ltok->next, nil);

                // _go back in time_
                jmpd = T;
                goto run_again;

            } else cmperr(MDINSTT, innr, nil);

            // TODO
        } else cmperr(EXPCTEX, innr, nil);
    
    // body holder
    } else if(eq_kwd_range(c, body)){
        if(eq_sym(c->next, SYM_BRA, 0)){
            push_node(pntr, c);

            pntr->next = parse(c->next->next, SCOPE);
            pntr->next->last = pntr;
            pntr = pntr->next;

            path->ltok = pntr->ltok;
            path->end  = pntr;
        // a body definition is obligated
        } else
        cmperr(EXPCTBD, c->next, &(tkn){.vall.str = OBS_IFB, .apdx = 1});

    // expression holders
    } else if(eq_kwd_range(c, hldr)){
        tkn *exp = c->next;
        // parentheses
        if(eq_sym(exp, SYM_PAR, 0)) exp = exp->next;

        // return statement
        if(c->vall.num == KW_RETURN){
            if(exp->type == LITERAL or exp->type == KEYWORD){
                pntr->next = validthnd(exp, F, F);
                pntr->next->last = pntr;
                pntr = pntr->next;

                path->ltok = exp;

                if(pntr == nil) cmperr(EXPVHND, exp, nil);
            } else cmperr(EXPVHND, exp, nil);

        // extrn statement
        } else if(c->vall.num == KW_EXTRN){
            free(path);
            free(pntr);
            return extrn_exp(c);

        // it's a goto statement
        } else if(c->vall.num == KW_GOTO){
            if(exp->type == INDEXER){
                push_node(pntr, exp);
                path->ltok = exp;
            // Unexpected symbol
            } else cmperr(UNEXPCT, exp, nil);
        }

    // single statements
    } else if(eq_kwd_range(c, sngl)){
        if(!eq_sym(c->next, SYM_CLN, 0)) cmperr(UNEXPCT, c->next, nil);
        else {
            path->ltok = c;
            path->end  = pntr;
            pntr->next = pntr; // loop up to itself
            pntr->last = pntr;
        }
    // function-like
    } else if(eq_kwd_range(c, funl)){
        if(!eq_sym(c->next, SYM_PAR, 0)) cmperr(UNEXPCT, c->next, nil);
        else {
            node *eval = funcall_r(c);
            pntr->next = eval->stt->next;
            pntr->next->last = pntr;

            pntr = eval->end;
            path->end = pntr;
            path->ltok = eval->ltok;

            // keywords are the only "functions" that only accepts exactly 1 arg
            if(eval->dcnt > 1) cmperr(TOOMUCH, eval->ltok, nil);
            else if(!eval->dcnt) cmperr(TOOFEWC, eval->ltok, nil);
        }
    }
    
    path->end = pntr;
    path->stt->last = path->end;
    path->end->next = path->stt;
    return path;
}
// function definition codepath rule
// the given token must be the opening parentheses of the args block 
node *fun_def_r(tkn *c){
    node *path = alloc(sizeof(node));
    node *pntr = alloc(sizeof(node));

    path->is_parent = T;
    path->stt  = pntr;
    path->type = FUNDEF;
    path->ftok = c->last;

    // end of arguments
    tkn *eoa = matchpair(c);
    
    // the body is a single line
    if(eoa->next->type == INDEXER
    or eoa->next->type == LITERAL){
        // TODO: handle single lined functions
        assert(F, "single-lined functions not implemented yet");

    // function call
    } else if(eq_sym(eoa->next, SYM_CLN, 0)){
        free(pntr);
        free(path);
        return funcall_r(c->last);

    // bracket scope
    } else if(eq_sym(eoa->next, SYM_BRA, 0)){
        path->ltok = matchpair(eoa->next);

    } else cmperr(UNEXPCT, c, nil);

    // start code path
    pntr->type = INDEXER;
    pntr->vall = c->last;
    pntr->is_parent = F;

    // args is not empty
    if(eoa != c->next){
        bool which = F, isptr = F;
        for(tkn *t = c->next; t != eoa; t = t->next){
            switch (t->type){
                case INDEXER:
                    if(!which){
                        which = T;
                        // default function arguments or computation on assignment
                        if(t->next->type == OPERATOR){
                            // do not allow operations on pointers
                            if(isptr) cmperr(NOPTRAR, t->next, nil);

                            if(!eq_opr_range(t->next, asgn))
                                cmperr(NOTASGN, t->next, nil);

                            node  *exp = exprss_r(t->next, F);
                            pntr->next = exp;
                            pntr->next->last = pntr;
                            pntr = exp->next;
                            t = exp->ltok;

                        // just the parameter
                        } else {
                            push_node(pntr, t);

                            // handle pointer definitions
                            if(isptr){
                                if(eq_sym(t->next, SYM_SQR, 1)){
                                    isptr = F;
                                    t = t->next;
                                }
                                pntr->type = PPARAM;
                                // skip this symbol

                            } else pntr->type = PARAMT;
                        }
                    }
                    else cmperr(UNEXPCT, t, nil);
                    break;
                case LSYMBOL:
                    // just a comma
                    if(t->vall.num == SYM_COM){
                        if(which) which = F;
                        else cmperr(UNEXPCT, t, nil); 
                        break;

                    // the parameter is a pointer
                    } else if(eq_sym(t, SYM_SQR, 0)){
                        if(!which) {
                            isptr = T;
                            continue;
                        // syntax error
                        } else cmperr(UNEXPCT, t,
                            &(tkn){.vall.str = PARAMPT, .apdx = 1}
                        );
                    }
                default:
                    cmperr(UNEXPCT, t, nil);
            }
        }
        // syntax errors
        if(!which) cmperr(UNEXPCT, eoa->last, nil);
        else if(isptr) cmperr(EXPPPAR, eoa,
            &(tkn){
                .vall.str = PRVONHR,
                .line = c->next->line,
                .coln = c->next->coln,
            }
        );
    }

    // validate the body of the function
    node *body = parse(eoa->next->next, SCOPE);
    path->ltok = body->ltok;
    path->end  = body;
    pntr->next = body;
    pntr->next->last = pntr;

    path->end->next = path->stt;
    path->stt->last = path->end;
    return path;
}
// function call codepath rule
// the given token may be the indexer
node *funcall_r(tkn *c){
    node *path = alloc(sizeof(node));
    node *pntr = alloc(sizeof(node));

    path->is_parent = T;
    path->stt = pntr;
    path->ftok = c;
    path->type = FNCALL;

    // end of arguments
    tkn *eoa = matchpair(c->next);

    // append function/keyword name
    pntr->type = c->type;
    pntr->vall = c;

    // assert keyword type
    if(pntr->type == KEYWORD){
        if(!eq_kwd_range(c, funl))
            cmperr(NOTFLKW, c, nil);

    // assert function name type
    } else if(pntr->type != INDEXER){
        cmperr(CALLINV, c, nil);
    }
    
    // the body is a single line
    if(eoa != c->next->next){
        // the function call args handling's just
        // like the function def params handling,
        // but you seek for exps or literals, not
        // assignments
        bool which = 0;
        for(tkn *t = c->next->next; t != eoa; t = t->next){
            switch (t->type){
                case INDEXER:
                case LITERAL:
                    if(!which){
                        which = T;
                        path->dcnt++;

                        // validate argument
                        pntr->next = validthnd(t, F, F);
                        if(pntr->next == nil) cmperr(EXPVHND, t, nil);
                        pntr->next->last = pntr;
                        pntr = pntr->next;

                        t = pntr->ltok;
                    
                    } else cmperr(UNEXPCT, t, nil); 
                    break;
                case LSYMBOL:
                    if(which) which = F;
                    else cmperr(UNEXPCT, t, nil); 

                    if(!eq_sym(t, SYM_COM, 0))
                        cmperr(UNEXPCT, t, nil);
                    break;
                default:
                    cmperr(UNEXPCT, t, nil);
            }
        }
    }

    path->ltok = eoa;
    path->end = pntr;

    path->end->next = path->stt;
    path->stt->last = path->end;
    return path;
}
// goto jump label codepath rule
node *labeldf_r(tkn *c, bool is_swedish){
    node  *path = nil;
    return path;
}
// goto jump codepath rule
node *jmp_stt_r(tkn *c){
    node  *path = nil;
    return path;
}

// handle invocation of functions and renaming of them
// expected path: extrn const [, other] [from <file>] [as name [, ...]]
// output STTMNT { EXTRN, [PATH], {IMPORT, ALIAS} }
node *extrn_exp(tkn *c){
    // just to be sure
    assert(c and c->type == KEYWORD
    and c->vall.num == KW_EXTRN, nil);

    node *path = alloc(sizeof(node));
    node *pntr = alloc(sizeof(node));

    path->type = STTMNT;
    path->ftok = c;
    path->stt  = pntr;

    pntr->type = KEYWORD;
    pntr->vall = c;

    // this node holds all imported namespaces
    node *i_ns = alloc(sizeof(node));
    node *chld = nil;
    i_ns->type = COLLEC;
    
    u16  block = 0;
    bool swtch = F;
    tkn  *next = c->next;
    while(T){
        if(eq_sym(next, SYM_CLN, 0)) break;
        // handle imported namespaces
        switch(block){
            // const [, other]
            case 0:
                // namespace
                if(next->type == INDEXER){
                    if(!swtch){
                        if(chld){
                            push_node(chld, next);
                        } else {
                            chld = alloc(sizeof(node));
                            i_ns->stt  = chld;
                            chld->type = INDEXER;
                            chld->vall = next;
                        }
                        path->ltok = next;
                        next = next->next;
                        swtch = T;
                    } else cmperr(UNEXPCT, next, nil);

                // collon
                } else if(eq_sym(next, SYM_COM, 0)){
                    if(swtch){
                        next = next->next;
                        swtch = F;
                    } else cmperr(UNEXPCT, next, nil);

                // next block
                } else if(next->vall.num == KW_FROM){
                    block++;
                    next = next->next;
                    // no need to push this token because the compiler
                    // already expects a string or the next block

                } else if(eq_sym(next, SYM_CLN, 0)){
                    goto outer_break; // End Of Statement

                } else cmperr(UNEXPCT, next, nil);
                break;
            // [from <file>]
            case 1:
                // path to file that defines the imported namespaces
                if(next->type == LITERAL and next->apdx == STRING){
                    // TODO: check if file exists
                    block++;
                    push_node(pntr, next);
                    path->ltok = next;
                    next = next->next;

                // unexpected token
                } else cmperr(UNEXPCT, next,
                    &(tkn){.vall.str = IPATH_E, .apdx = 1}
                );
                break;
            // [as name [, ...]]
            case 2:
                if(next->type == KEYWORD and next->vall.num == KW_AS){
                   block++;
                   i_ns->next = alloc(sizeof(node));
                   i_ns = i_ns->next;
                   i_ns->type = MIDREP;

                   next = next->next;
                   swtch = F;

                // End Of Statement
                } else if(eq_sym(next, SYM_CLN, 0)){
                    goto outer_break;
                
                // unexpected symbol
                } else cmperr(UNEXPCT, next, nil);
                break;

            case 3:
                // namespace
                if(next->type == INDEXER){
                    if(!swtch){
                        push_node(chld, next);

                        path->ltok = next;
                        next = next->next;
                        swtch = T;
                    } else cmperr(UNEXPCT, next, nil);

                // collon
                } else if(eq_sym(next, SYM_COM, 0)){
                    if(swtch){
                        next = next->next;
                        swtch = F;
                    } else cmperr(UNEXPCT, next, nil);

                } else if(eq_sym(next, SYM_CLN, 0)){
                    goto outer_break; // End Of Statement

                // unexpected token
                } else cmperr(UNEXPCT, next, nil);

            continue;
            outer_break: break;
        }
    }
    i_ns->end  = chld;
    pntr->next = i_ns;
    path->end  = pntr;

    path->stt->last = path->end;
    path->end->next = path->stt;
    return path;
}

// validates path as a righthand or lefthand if ``is_nmsc`` is defined
node *validthnd(tkn *c, bool is_nmsc, bool unwinding){
    node *path = alloc(sizeof(node));
    node *pntr = alloc(sizeof(node));

    path->ftok = c;

    pntr->type = c->type;
    pntr->vall = c;
    pntr->ltok = c;

    // value is an array indexing
    if(eq_sym(c->next, SYM_SQR, 0)){
        path->type = IDXING;
        path->vall = c;
        path->stt  = pntr;
        path->is_parent = T;

        tkn *innr = c->next->next;

        // this path fallows the syntax: foo[bar + ...] 
        if(innr->next->type == OPERATOR){
            pntr->next = exprss_r(innr->next, F);
            pntr->next->last = pntr;
            pntr = pntr->next;
        // this path fallows the syntax: foo[+bar ...]
        } else if(innr->type == OPERATOR){
            pntr->next = exprss_r(innr, F);
            pntr->next->last = pntr;
            pntr = pntr->next;
        // only a value indexing
        } else if(innr->type == INDEXER or innr->type == LITERAL){
            push_node(pntr, innr);
            pntr->ltok = innr;

        // unexpected token
        } else cmperr(INVLDIX, innr, nil);

        // assert syntax
        if(!eq_sym(pntr->ltok->next, SYM_SQR, 1))
            cmperr(UNEXPCT, pntr->ltok->next, nil);
        // move ltok pointer forward
        else pntr->ltok = pntr->ltok->next;

        path->end = pntr;
        path->ltok = pntr->ltok;
        return path;
    
    // it's a left-hand and it's an array (indexing)
    } else if(eq_sym(c, SYM_SQR, 1)){
        free(path);
        free(pntr);
        tkn *bgn = matchpair(c);
        return validthnd(bgn->last, T, F);

    // structure literal assignment
    } else if(c->type == INDEXER and eq_sym(c->next, SYM_BRA, 0)){
        free(path);
        free(pntr);
        return struct_r(c);

    // function call or function declaration
    } else if(eq_sym(c->next, SYM_PAR, 0)){
        free(path);
        free(pntr);
        return fun_def_r(c->next);

    // function return
    } else if(eq_sym(c, SYM_PAR, 1)){
        free(path);
        free(pntr);
        tkn *bgn = matchpair(c);
        return funcall_r(bgn->last);

    // accessing field
    } else if(eq_sym(c->next, SYM_DOT, 0)){
        
        path->stt  = pntr;
        path->type = ACCESS;
        path->is_parent = T;

        // fields must be indexers
        if(c->next->next->type == INDEXER){
            // push dot to later information
            push_node(pntr, c->next);

            pntr->next = validthnd(c->next->next, T, T);

            if(pntr->next == nil)
                cmperr(INVLDAC, c->next->next, nil);
            // push forward
            else {
                pntr->next->last = pntr;
                pntr = pntr->next;

                path->ltok = pntr->ltok;
                path->end  = pntr;

                return path;
            }
        // syntax error
        } else cmperr(UNEXPCT, c->next->next, nil);

    } else if(c->type == INDEXER){
        free(path);
        // the indexer is a field
        if(eq_sym(c->last, SYM_DOT, 0) and !unwinding){
            tkn *root = c->last->last;

            // go back til the first field
            while(eq_sym(root->last, SYM_DOT, 0)){
                root = root->last->last;
                if(root->type != INDEXER) cmperr(UNEXPCT, root, nil);
            }
            
            free(pntr);
            return validthnd(root, T, T);
        // just a single indexer
        } else {
            return pntr;
        }

    // single normal value
    } else if(!is_nmsc){
        if(c->type == LITERAL){
            free(path);
            return pntr;
        // true and function-like statements are valid right-hands
        } else if(eq_kwd_range(c, funl)){
            free(path);
            free(pntr);
            return funcall_r(c);

        } else if(eq_kwd_range(c, trus)){
            // TODO: implement true statements as expressions
            assert(F, "statements as values are not implemented yet");
        }
    // the caller must handle the error
    }

    return nil;
}

// matches the closing or opening token index of a given symbol
tkn *matchpair(tkn *c){
    u64 cnt = 0;
    assert(c->type == LSYMBOL, get_tokval(c));

    // it's a opening symbol
    if(c->apdx == 0){
        for(tkn *t = c; t->next != EOTT; t = t->next){
            if(t->type == LSYMBOL and eq_sym(t, c->vall.num, t->apdx)){
                // if different, it's the pair
                if(t->apdx != c->apdx) cnt--;
                else cnt++;

                // if scope level matches, return
                if(cnt == 0) return t;
            }
        }
    // it's a closing one
    } else {
        for(tkn *t = c; t->last != EOTT; t = t->last){
            // it's a symbol
            if(t->type == LSYMBOL){
                // it's the pair that we're looking for
                if(t->vall.num == c->vall.num){
                    // if different, it's the pair
                    if(t->apdx != c->apdx) cnt--;
                    else cnt++;

                    // if scope level matches, return
                    if(cnt == 0) return t;
                }
            }
        }
    }
    assert(F, "could not find matching pair");
}

// checks if the current path is terminated with a semicolon
void hasscolon(node *out){
    assert(out != nil && out->ltok, "invalid node to evaluate");
    // is an end of line?
    if(!eq_sym(out->ltok->next, SYM_CLN, 0))
        cmperr(NOTERMN, out->ltok, nil);
}

// returns the given node as a string
char *nodet_to_str(node *n){
    char  *out = alloc(128);
    strcpy(out, "");
    switch(n->type){ 
        case KEYWORD :
            sprintf(out, "KEYWORD [%s]", KEYWORDS[n->vall->vall.num]);
            break;
        case INDEXER :
            sprintf(out, "INDEXER [%s]", n->vall->vall.str);
            break;
        case LITERAL :
            if(n->vall->apdx == STRING)
                sprintf(out, "LITERAL [\"%s\"]", sarr.arr[n->vall->vall.num]);
            else
                sprintf(out, "LITERAL [%lx]", n->vall->vall.num);
            break;
        case LSYMBOL :
            if(n->vall->apdx == 0)
                sprintf(out, "SYMBOL [%s]", SYMBOLS[n->vall->vall.num].s);
            else
                sprintf(out, "SYMBOL [%s]", SYMBOLS[n->vall->vall.num].e);
            break;
        case OPERATOR:
            if(n->vall->apdx == 0)
                sprintf(out, "OPERATOR [%s]", OPERATORS[n->vall->vall.num]);
            else
                sprintf(out, "OPERATOR [%s]", TXT_OPERS[n->vall->vall.num]);
            break;
        case CONSTD  :
            strcpy(out, "CONST. DEF.");
        case DEFINE  :
            strcpy(out, "NAMESPACE DEF.");
            break;
        case ASSIGN  :
            strcpy(out, "ASSIGNMENT");
            break;
        case ARRDEF  :
            strcpy(out, "ARRDEF");
            break;
        case STRDEF  :
            strcpy(out, "STRUCTURE DEF");
            break;
        case ENUMDF  :
            strcpy(out, "ENUM DEF.");
            break;
        case STRUCT  :
            strcpy(out, "STRUCT");
            break;
        case STTMNT  :
            strcpy(out, "STATEMENT");
            break;
        case EXPRSS  :
            strcpy(out, "EXPRESION");
            break;
        case LABELD  :
            strcpy(out, "LABEL");
            break;
        case JMPSTT  :
            strcpy(out, "JUMP");
            break;
        case FUNDEF  :
            strcpy(out, "FUNCTION DEF.");
            break;
        case FNCALL  :
            strcpy(out, "FUNCTION CALL");
            break;
        case IDXING  :
            strcpy(out, "ARRAY INDEXING");
            break;
        case ACCESS  :
            strcpy(out, "FIELD ACCESS");
            break;
        case BODYDF  :
            strcpy(out, "BODY BEGIN");
            break;
        case EOSCPE  :
            strcpy(out, "END OF SCOPE");
            break;
        case PARAMT  :
            sprintf(out, "PARAM [%s]", n->vall->vall.str);
            break;
        case PPARAM  :
            sprintf(out, "POINTER PARAM [%s]", n->vall->vall.str);
            break;
        case CODEIS  :
            strcpy(out, "INIT CODE");
            break;
        case CDHALT  :
            strcpy(out, "HALT");
            break;
        default:
            sprintf(out, "UNKNOWN [%d/%s]", n->type, get_tokval(n->vall));
            break;
    }
    return out;
}

node *parse(tkn *tkns, cmod mode){
    // if any rule breaks, retreat parsing until here
    tkn  *ctok = tkns, *eos;
    node *ctxt = alloc(sizeof(node));
    node *pntr = nil;
    ctxt->stt  = nil;

    ctxt->is_parent = T;
    ctxt->type = (mode == SCOPE ? BODYDF : CODEIS);

    // config code chuck
    if(mode == SCOPE){
        eos = matchpair(tkns->last);
        ctxt->type = BODYDF;
    } else {
        eos = EOTT;
        ctxt->type = CODEIS;
    }

    bool mtch = T;
    tkn *tok, *old = nil;

    void *lbls[] = {
        &&case_KEYWORD ,
        &&case_INDEXER ,
        &&case_LITERAL ,
        &&case_LSYMBOL ,
        &&case_OPERATOR,
        &&case_EOTT
    };
    #define nxt(t)\
        t = t->next;\
        goto *lbls[t->type];
        //printf("%s\n", get_tokval(t));

    tok = tkns;
    goto *lbls[tok->type];

    // statements and definitions
    case_KEYWORD:
        // local definition
        if(eq_kwd_range(tok, ldef)){
            // avoid memory leaking
            if(ctxt->stt){
                pntr->next = define_r(tok);
                pntr->next->last = pntr;
                pntr = pntr->next;
            } else {
                ctxt->stt = define_r(tok);
                pntr = ctxt->stt;
            }

            // there is an end of line
            hasscolon(pntr);
            tok = pntr->ltok;

        // statements
        } else if(eq_kwd_range(tok, sttt)){
            // avoid memory leaking
            if(ctxt->stt){
                pntr->next = sttmnt_r(tok);
                pntr->next->last = pntr;
                pntr = pntr->next;
            } else {
                ctxt->stt = sttmnt_r(tok);
                pntr = ctxt->stt;
            }

            // evaluate if tails
            while((eq_kwd(pntr->ltok->next, KW_IF) or
            eq_kwd(pntr->ltok->next, KW_ELIF))
            and eq_kwd_range(pntr->ltok->next, iftl)){

                pntr->next = sttmnt_r(pntr->ltok->next);
                pntr->next->last = pntr;
                pntr = pntr->next;

                printf("%s %d %d\n", 
                    get_tokval(pntr->ltok->next),
                    pntr->ltok->next->line,
                    pntr->ltok->next->coln
                );
            }

            // there is an end of line?
            hasscolon(pntr);
            tok = pntr->ltok;

        // otherwise is not
        } else cmperr(UNEXPCT, tok, nil);
        nxt(tok);


    // if it's an indexer, it's a constant definition
    // or an assignment, so check for both cases.
    case_INDEXER:
        // skip this so then the next case can take care
        if(tok->next->type == OPERATOR){
            nxt(tok);
        } else {
            // avoid memory leaking
            if(ctxt->stt){
                pntr->next = constd_r(tok);
                pntr->next->last = pntr;
                pntr = pntr->next;
            } else {
                ctxt->stt = constd_r(tok);
                pntr = ctxt->stt;
            }

            // there is an end of line?
            hasscolon(pntr);
            tok = pntr->ltok;
        }
        nxt(tok);

    case_LITERAL:
        // if some literal value scaped a expression or statement it's invalid
        cmperr(UNEXPCT, tok, nil);

    case_LSYMBOL:
        if(eq_sym(tok, SYM_BRA, 0)){
            // avoid memory leaking
            if(ctxt->stt){
                pntr->next = parse(tok, SCOPE);
                pntr->next->last = pntr;
                pntr = pntr->next;
            } else {
                ctxt->stt = parse(tok, SCOPE);
                pntr = ctxt->stt;
            }

            // there is an end of line?
            hasscolon(pntr);
            tok = pntr->ltok;

        // it's the end of the scope
        } else if(eq_sym(tok, SYM_BRA, 1)){
            // finish if it's the end and we're within a scope
            if(mode == SCOPE){
                if(matchpair(tok) == tkns){
                    goto *lbls[UNKNOWN];
                } else {
                    nxt(tok);
                }
            }
            else cmperr(UNEXPCT, tok, nil);

        // just a terminator, next token!
        } else if(!eq_sym(tok, SYM_CLN, 0))
            cmperr(UNEXPCT, tok, nil);

        nxt(tok);

    // expressions
    case_OPERATOR:
        // an assignment is allowed out of an statement
        if(eq_opr_range(tok, asgn)){
            // avoid `+foo = bar` be allowed
            if(!(eq_sym(tok->last->last, SYM_CLN, 0)
            or tok->last->last == EOTT))
                cmperr(UNEXPCT, tok, nil);
            
            // avoid memory leaking
            if(ctxt->stt){
                pntr->next = assign_r(tok, F);
                pntr->next->last = pntr;
                pntr = pntr->next;
            } else {
                ctxt->stt = assign_r(tok, F);
                pntr = ctxt->stt;
            }

            // there is an end of line?
            hasscolon(pntr);
            tok = pntr->ltok;

        // other operations are not
        } else cmperr(ALONEXP, tok, nil);
        nxt(tok);

    case_EOTT:
        assert(tok == EOTT, nil);

        pntr->next = alloc(sizeof(node));
        pntr->next->last = pntr;
        pntr = pntr->next;

        pntr->type = (mode == SCOPE ? EOSCPE : CDHALT);
        pntr->is_parent = F;

        ctxt->ltok = eos;
        ctxt->end  = pntr;

    return ctxt;
}