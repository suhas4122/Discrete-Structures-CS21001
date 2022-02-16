/***************
SUHAS JAIN 
19CS30048
DISCRETE STRUCTURES 
CODING ASSIGNMENT - 2
RECURRENCE RELATION SOLVER
****************/

#include<bits/stdc++.h>
using namespace std;

typedef struct splnum{
	int a;
	int b;
	int c;
	int d;
}splnum;

void printspl(splnum s){
	int a = s.a;
	int b = s.b;
	int c = s.c;
	int d = s.d;
	if(b==0 || d==0){
		if(a==0){
			cout<<0;
			return;
		}
		cout<<a;
		if(c!=1)	cout<<" / "<<c;
		return;
	}
	if(c!=1)	cout<<"(";
	if(a!=0)	cout<<a<<" ";
	char c1 = b>0?'+':'-';
	b = abs(b);
	if(a!=0 || c1=='-')	cout<<c1;
	if(b!=1 && b!=-1)	cout<<" "<<b;
	if(a!=0)	cout<<" ";
	if(d!=1)	cout<<"sqrt("<<d<<")";
	if(c!=1)	cout<<") / "<<c;
	return;
}

int GCD(int a, int b){ 
    if (a == 0) 
        return b; 
    return GCD(b % a, a); 
} 

int gcd(int a, int b, int c){ 
	a = abs(a);
	b = abs(b);
	c = abs(c);
    int result = a;
    result = GCD(b, result); 
    result = GCD(c, result);  
    return abs(result); 
}

bool checkequal(splnum a, splnum b){
	if(a.a==b.a && a.b==b.b && a.c==b.c){
		if(a.b==0) return true;
		else return(a.d==b.d);
	}
	return false;
}

bool isSquare(int x){
	if(x == 0) return true;
	int sr = sqrt(x); 
  	return (x - sr*sr == 0); 
} 

void simplify(splnum &u){
	if(u.c<0){
		u.c *= -1;
		u.a *= -1;
		u.b *= -1;
	}
	int gcd1 = gcd(u.a, u.b, u.c);
	u.a /= gcd1;
	u.b /= gcd1;
	u.c /= gcd1;
	if(isSquare(u.d) && u.d>0){
		u.a = u.a+u.b*sqrt(u.d);
		u.b = u.d = 0;
		int gcd2 = GCD(u.a, u.c);
		u.a /= gcd2;
		u.c /= gcd2;
	}
	if(u.a==0 && u.b!=0 && u.c!=0){
		int gcd2 = GCD(u.b, u.c);
		u.b /= gcd2;
		u.c /= gcd2;
	}
}

splnum addspl(splnum u, splnum v){
	if(u.d != v.d)	cout<<"Error!!";
	splnum add;
	add.a = v.c*u.a + u.c*v.a;
	add.b = v.c*u.b + u.c*v.b;
	add.d = u.d;
	add.c = u.c*v.c;
	simplify(add);
	return add;
}

splnum subspl(splnum u, splnum v){
	if(u.d != v.d)	cout<<"Error!!";
	if(u.a==0 && u.b==0){
		v.a *= -1;
		v.b *= -1;
		return v;
	}
	splnum sub;
	sub.a = v.c*u.a - u.c*v.a;
	sub.b = v.c*u.b - u.c*v.b;
	sub.d = u.d;
	sub.c = u.c*v.c;
	simplify(sub);
	return sub;
}

splnum mulspl(splnum u, splnum v){
	if(u.d != v.d)	cout<<"Error!!";
	splnum mul;
	mul.a = u.a*v.a + u.b*v.b*u.d;
	mul.b = v.a*u.b + u.a*v.b;
	mul.d = u.d;
	mul.c = u.c*v.c;
	simplify(mul);
	return mul;
}

splnum invspl(splnum u){
	splnum inv;
	inv.a = u.a*u.c;
	inv.b = -1*u.b*u.c;
	inv.c = u.a*u.a - u.b*u.b*u.d;
	inv.d = u.d;
	simplify(inv);
	return inv;
}

splnum divspl(splnum u, splnum v){
	if(u.d != v.d)	cout<<"Error!!";
	splnum inv = invspl(v);
	return mulspl(inv, u);
}	

void findroots(int r, int s, splnum *roots){
	int deter = r*r + 4*s;
	if(isSquare(deter) && deter>=0){
		roots[0].a = (r+sqrt(deter))/2;
		roots[0].b = roots[0].d = 0;
		roots[0].c = 1;
		roots[1].a = (r-sqrt(deter))/2;
		roots[1].b = roots[1].d = 0;
		roots[1].c = 1;
	}
	else{
		roots[0].a = r;
		roots[0].b = 1;
		roots[0].c = 2;
		roots[0].d = deter;
		roots[1].a = r;
		roots[1].b = -1;
		roots[1].c = 2;
		roots[1].d = deter;
	}
	if(roots[0].d%4 == 0){
		roots[0].d /= 4;
		roots[0].b *= 2;
	}
	if(roots[1].d%4 == 0){
		roots[1].d /= 4;
		roots[1].b *= 2;
	}
	simplify(roots[0]), simplify(roots[1]);
}

void solvehomogenous(int r, int s, int a0, int a1){
	splnum *roots = new splnum[2];
	splnum coeff[2];
	findroots(r, s, roots);
	cout<<"\tCharateristic equation: x^2 + ("<<-1*r<<")x + ("<<-1*s<<") = 0\n";
	cout<<"\tRoot 1 = ";
	printspl(roots[0]), cout<<endl;
	cout<<"\tRoot 2 = ";
	printspl(roots[1]), cout<<endl;
	cout<<"\tHomogenous solution: \n";

	if(!checkequal(roots[0], roots[1])){
		splnum num1 = {a0, 0, 2, r*r + 4*s};

		splnum num2;
		if((2*a1 - r*a0) != 0){
			num2.a = 0;
			num2.b = 2;
			num2.c = 2*a1 - r*a0;
			num2.d = r*r + 4*s;
		}
		else{
			num2 = {0, 0, 1, 0};
		}
		num2 = invspl(num2);

		simplify(num1);
		simplify(num2);

		coeff[0] = addspl(num1, num2);
		coeff[1] = subspl(num1, num2);

		if(coeff[0].d%4 == 0){
			coeff[0].d /= 4;
			coeff[0].b *= 2;
		}
		if(roots[1].d%4 == 0){
			coeff[1].d /= 4;
			coeff[1].b *= 2;
		}
		simplify(coeff[0]);
		simplify(coeff[1]);

		cout<<"\t[";
		printspl(coeff[0]);
		cout<<"]"<<"[";
		printspl(roots[0]);
		cout<<"]"<<"^n + [";
		printspl(coeff[1]);
		cout<<"][";
		printspl(roots[1]);
		cout<<"]^n"<<endl;
	}
	else{
		splnum num1 = {a0, 0, 1, 0};
		splnum num2 = {a1, 0, 1, 0};
		coeff[0] = num1;
		coeff[1] = divspl(num2, roots[0]);
		coeff[1] = subspl(coeff[1], num1);

		if(coeff[0].d%4 == 0){
			coeff[0].d /= 4;
			coeff[0].b *= 2;
		}
		if(roots[1].d%4 == 0){
			coeff[1].d /= 4;
			coeff[1].b *= 2;
		}
		simplify(coeff[0]);
		simplify(coeff[1]);

		cout<<"\t[(";
		printspl(coeff[1]);
		cout<<")n + (";
		printspl(coeff[0]);
		cout<<")][";
		printspl(roots[0]);
		cout<<"]^n"<<endl;
	}
}

void solvenonhomogenous(int r, int s, int t, int a0, int a1){
	splnum *roots = new splnum[2];
	splnum coeff[3];
	findroots(r, s, roots);

	cout<<"\tCharateristic equation: x^2 + ("<<-1*r<<")x + ("<<-1*s<<") = 0\n";
	cout<<"\tRoot 1 = ";
	printspl(roots[0]), cout<<endl;
	cout<<"\tRoot 2 = ";
	printspl(roots[1]), cout<<endl;

	int d = roots[0].d;
	splnum one = {1, 0, 1, d};
	splnum c0 = {a0, 0, 1, d};
	splnum c1 = {a1, 0, 1, d};

	if(!checkequal(roots[0], roots[1]) && !checkequal(one, roots[0]) && !checkequal(one, roots[1])){
		cout<<"\tParticular solution : ";
		splnum particular = {-t, 0, r+s-1, d};
		simplify(particular);
		printspl(particular);
		coeff[0] = subspl(c1, mulspl(c0, roots[1]));
		coeff[0] = addspl(coeff[0], mulspl(particular, subspl(roots[1], one)));
		coeff[0] = divspl(coeff[0], subspl(roots[0], roots[1]));
		coeff[1] = subspl(c1, mulspl(c0, roots[0]));
		coeff[1] = addspl(coeff[1], mulspl(particular, subspl(roots[0], one)));
		coeff[1] = divspl(coeff[1], subspl(roots[1], roots[0]));

		cout<<"\n\tHomogenous solution : \n";
		cout<<"\t[";
		printspl(coeff[0]);
		cout<<"][";
		printspl(roots[0]);
		cout<<"]"<<"^n + [";
		printspl(coeff[1]);
		cout<<"][";
		printspl(roots[1]);
		cout<<"]^n";	
	}
	else if(!checkequal(roots[0], roots[1]) && checkequal(one, roots[0])){
		splnum r1 = roots[1];
		cout<<"\tParticular solution : ";
		splnum particular = {t, 0, r+2*s, d};
		simplify(particular);
		cout<<"[", printspl(particular), cout<<"] n";
		coeff[0] = subspl(subspl(c1, c0), particular);
		coeff[0] = divspl(coeff[0], subspl(r1, one));
		coeff[1] = subspl(mulspl(c0, r1), c1);
		coeff[1] = addspl(coeff[1], particular);
		coeff[1] = divspl(coeff[1], subspl(r1, one));

		cout<<"\n\tHomogenous solution : \n";
		cout<<"\t[";
		printspl(coeff[0]);
		cout<<"][";
		printspl(roots[0]);
		cout<<"]^n + [";
		printspl(coeff[1]);
		cout<<"][";
		printspl(roots[1]);
		cout<<"]^n";	
	}
	else if(!checkequal(roots[0], roots[1]) && checkequal(one, roots[1])){
		splnum r1 = roots[0];
		cout<<"\tParticular solution : ";
		splnum particular = {t, 0, r+2*s, d};
		simplify(particular);
		cout<<"[", printspl(particular), cout<<"] n";
		coeff[0] = subspl(subspl(c1, c0), particular);
		coeff[0] = divspl(coeff[0], subspl(r1, one));
		coeff[1] = subspl(mulspl(c0, r1), c1);
		coeff[1] = addspl(coeff[1], particular);
		coeff[1] = divspl(coeff[1], subspl(r1, one));

		cout<<"\n\tHomogenous solution : \n";
		cout<<"\t[";
		printspl(coeff[0]);
		cout<<"][";
		printspl(roots[0]);
		cout<<"]^n + [";
		printspl(coeff[1]);
		cout<<"][";
		printspl(roots[1]);
		cout<<"]^n";	
	}
	else if(checkequal(roots[0], roots[1]) && !checkequal(one, roots[0])){
		cout<<"\tParticular solution : ";
		splnum particular = {-t, 0, r+s-1, d};
		simplify(particular);
		printspl(particular);
		coeff[0] = subspl(c0, particular);
		coeff[1] = divspl(subspl(c1, particular), roots[1]);
		coeff[1] = subspl(coeff[1], coeff[0]);

		cout<<"\n\tHomogenous solution : \n";
		cout<<"\t[(";
		printspl(coeff[1]);
		cout<<")n + (";
		printspl(coeff[0]);
		cout<<")][";
		printspl(roots[1]);
		cout<<"]^n";
	}
	else{
		splnum r1 = roots[0];
		cout<<"\tParticular solution : ";
		splnum particular = {-1*t, 0, r+4*s, d};
		simplify(particular);
		cout<<"[", printspl(particular), cout<<"] n^2";
		coeff[0] = c0;
		coeff[1] = divspl(subspl(c1, particular), roots[1]);
		coeff[1] = subspl(coeff[1], coeff[0]);

		cout<<"\n\tHomogenous solution : \n";
		cout<<"\t[(";
		printspl(coeff[1]); 
		cout<<")n + (";
		printspl(coeff[0]);
		cout<<")][";
		printspl(roots[1]);
		cout<<"]^n";	
	}
}

int main(){
	int r, s, t, a0, a1;
	cout<<"r = ", cin>>r;
	cout<<"\ns = ", cin>>s;
	cout<<"\nt = ", cin>>t;
	cout<<"\na0 = ", cin>>a0;
	cout<<"\na1 = ", cin>>a1;
	cout<<"\n+++ Solving the homogenous recurrence\n";
	solvehomogenous(r, s, a0, a1);
	cout<<"\n+++ Solving the nonhomogenous recurrence\n";
	solvenonhomogenous(r, s, t, a0, a1);
	return 0;
}

