#include "ext.h"
#include "ext_obex.h"

typedef struct _lorenz
{
	t_object l_ob;
    
	double l_xyz[3];
    double l_h;
	
	void *l_out0;
	void *l_out1;
	void *l_out2;
					
} t_lorenz;

void lorenz_bang(t_lorenz *x);
void lorenz_float(t_lorenz *x, float n);   
void lorenz_ft1(t_lorenz *x, float o);
void lorenz_ft2(t_lorenz *x, float p);
void lorenz_ft3(t_lorenz *x, float q);
void lorenz_assist(t_lorenz *x, void *b, long m, long a, char *s);
void lorenz_inletinfo(t_lorenz *x, void *b, long index, char *t);
void *lorenz_new(t_symbol *s, long argc, t_atom *argv);

static t_class *lorenz_class;


void ext_main(void *r)
{
    t_class *c;
    c = class_new("lorenz", (method)lorenz_new, (method)0L, sizeof(t_lorenz), 0L, A_GIMME, 0);
	
    class_addmethod(c, (method)lorenz_bang, "bang", 0);
	class_addmethod(c, (method)lorenz_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)lorenz_ft1, "ft1", A_FLOAT, 0);
	class_addmethod(c, (method)lorenz_ft2, "ft2", A_FLOAT, 0);
	class_addmethod(c, (method)lorenz_ft3, "ft3", A_FLOAT, 0);
    class_addmethod(c, (method)lorenz_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)lorenz_inletinfo, "inletinfo", A_CANT, 0);
	
	post("lorenz object by Michael F. Zbyszyński, v2.0 ©2002-2016");
	
    class_register(CLASS_BOX, c);
    lorenz_class = c;
}

void lorenz_bang(t_lorenz *x)
{
	float xnew;
	float ynew;
	float znew;

	znew = x->l_xyz[2]+(x->l_h * (( x->l_xyz[0] * x->l_xyz[1]) - ((8.0/3.0) * x->l_xyz[2])));
	outlet_float(x->l_out2, znew);
	
	ynew = x->l_xyz[1] + (x->l_h * ((28.0 * x->l_xyz[0] - x->l_xyz[1]) - ( x->l_xyz[0] * x->l_xyz[2])));
	outlet_float(x->l_out1, ynew);
	
	xnew = x->l_xyz[0] + ((x->l_h * 10.0) * (x->l_xyz[1] - x->l_xyz[0]));
	outlet_float(x->l_out0, xnew);
	
	x->l_xyz[0] = xnew;
	x->l_xyz[1] = ynew;
	x->l_xyz[2] = znew;
}

void lorenz_float(t_lorenz *x, float n)
{
	x->l_xyz[0] = n;
    lorenz_bang(x);
}

void lorenz_ft1(t_lorenz *x, float n)
{
	x->l_xyz[1] = n;
}

void lorenz_ft2(t_lorenz *x, float n)
{
	x->l_xyz[2] = n;
}

void lorenz_ft3(t_lorenz *x, float q)
{
	x->l_h = q;
}

void lorenz_assist(t_lorenz *x, void *b, long m, long a, char *s) // 4 final arguments are always the same for the assistance method
{
    if (m == ASSIST_OUTLET)
        switch (a) {
            case 0:
                sprintf(s,"X result of lorenz attractor");
                break;
            case 1:
                sprintf(s,"Y result of lorenz attractor");
                break;
            case 2:
                sprintf(s,"Z result of lorenz attractor");
                break;
        }
    else {
        switch (a) {
            case 0:
                sprintf(s,"float X or bang");
                break;
            case 1:
                sprintf(s,"float Y");
                break;
            case 2:
                sprintf(s,"float Z");
                break;
            case 3:
                sprintf(s,"float H");
                break;
        }
    }
}

void lorenz_inletinfo(t_lorenz *x, void *b, long index, char *t)
{
    if (index > 0) {
        *t = 1;
    }
}


void *lorenz_new(t_symbol *s, long argc, t_atom *argv)
{
    t_lorenz *x;
    x = (t_lorenz *)object_alloc(lorenz_class);
    x->l_xyz[0]= x->l_xyz[1] = x->l_xyz[2] = 0.6;
    x->l_h = 0.01;
    if (argc) {
        for (int i = 0; i < argc; ++i) {
            if (i < 3) {
                if ((argv+i)->a_type == A_FLOAT) {
                    x->l_xyz[i] = (double)atom_getfloat(argv+i);
                } else if ((argv+i)->a_type == A_LONG) {
                    x->l_xyz[i] = (double)atom_getlong(argv+i);
                }
            }
        }
        if (argc > 3){
            if (argv[3].a_type == A_FLOAT) {
                x->l_h = atom_getfloat(argv+3);
            } else if (argv[3].a_type == A_LONG) {
                x->l_h = (float)atom_getlong(argv+3);
            }
        }
    }
    
    floatin(x,3);
	floatin(x,2);
	floatin(x,1);
	
	x->l_out2 = floatout(x);
	x->l_out1 = floatout(x);
	x->l_out0 = floatout(x);
	
    return (x);
}

	
	

	