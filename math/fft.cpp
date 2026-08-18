typedef pair<long long,long long> ii;

const long long MAXN=1<<20; //watch out with RTEs (increase MAXN):
typedef vector<long long> poly;
struct CD {
	double r,i;
	CD(double r=0, double i=0):r(r),i(i){}
	double real()const{return r;}
	void operator/=(const int c){r/=c, i/=c;}
};
CD operator*(const CD& a, const CD& b){
	return CD(a.r*b.r-a.i*b.i,a.r*b.i+a.i*b.r);}
CD operator+(const CD& a, const CD& b){return CD(a.r+b.r,a.i+b.i);}
CD operator-(const CD& a, const CD& b){return CD(a.r-b.r,a.i-b.i);}
const double pi=acos(-1.0);

CD cp1[MAXN+9],cp2[MAXN+9];
int R[MAXN+9];
void dft(CD* a, int n, bool inv){
	for(int i = 0; i < n; i++) if(R[i]<i)swap(a[R[i]],a[i]);
	for(int m=2;m<=n;m*=2){
		double z=2*pi/m*(inv?-1:1);
		CD wi=CD(cos(z),sin(z)); 
		for(int j=0;j<n;j+=m){
			CD w(1);
			for(int k=j,k2=j+m/2;k2<j+m;k++,k2++){
				CD u=a[k];CD v=a[k2]*w;a[k]=u+v;a[k2]=u-v;w=w*wi;
			}
		}
	}
	if(inv) for(int i = 0; i < n; i++) a[i]/=n; 
}
poly multiply(poly& p1, poly& p2){
	int n=p1.size()+p2.size()+1;
	int m=1,cnt=0;
	while(m<=n)m+=m,cnt++;
	for(int i = 0; i < m; i++){R[i]=0;for(int j = 0; j < cnt; j++) R[i]=(R[i]<<1)|((i>>j)&1);}
	for(int i = 0; i < m; i++) cp1[i]=0,cp2[i]=0;
	for(int i = 0; i < (int)p1.size(); i++) cp1[i]=p1[i];
	for(int i = 0; i < (int)p2.size(); i++) cp2[i]=p2[i];
	dft(cp1,m,false);dft(cp2,m,false);
	for(int i = 0; i < m; i++) cp1[i]=cp1[i]*cp2[i];
	dft(cp1,m,true);
	poly res;
	n-=2;
	for(int i = 0; i < n; i++) res.push_back((long long)floor(cp1[i].real()+0.5));
	return res;
}

void getBigNumMulti(vector <long long> &c){ //Big numbers multiplication.
	vector <char> r;
	while(!c.empty()&&!c.back()) c.pop_back(); //quitar todos los 0 extras.
	if(c.empty()){
		cout<<0<<'\n';
		return;
	}
	long long x=0;
	//Normalizar los coeficientes para representarlos como digitos.
	for(int i = 0; i < (int)c.size(); i++){
		x+=c[i];
		r.push_back((char)(x%10) + '0');
		x/=10;
	}
	while(x){ //carry que sobra.
		r.push_back((char)(x%10) + '0');
		x/=10;
	}
	reverse(r.begin(), r.end());
	for(int i = 0; i < (int)r.size(); i++) cout<<r[i];
	cout<<'\n';
}

void stringMatchShift(){ //All possible scalar products with strings.
	string s;
	cin>>s;
	int n = s.size();
	vector <long long> a1(n,0),a2(2*n,0),b1(n,0),b2(2*n,0),c1(n,0),c2(2*n,0);
	vector <long long> ra,rb,rc;

	//Create binary polynomial for each letter.
	for(int i = 0; i < n; i++){
		if (s[i] == 'a') a1[i] = 1;
		else if (s[i] == 'b') b1[i] = 1;
		else c1[i] = 1;
	}
	//Make the dup for each letter to multiply:
	for(int i = 0; i < n; i++){
		a2[i] = a2[i+n] = a1[i];
		b2[i] = b2[i+n] = b1[i];
		c2[i] = c2[i+n] = c1[i];
	}
	//Append the rest of the zeros (Step 1):
	for(int i = 0; i < n; i++){
		a1.push_back(0), b1.push_back(0), c1.push_back(0);
	}
	//Reverse the arrays (Step 2):
	reverse(a1.begin(), a1.end());
	reverse(b1.begin(), b1.end());
	reverse(c1.begin(), c1.end());
	//Multiply the polynomials:
	ra = multiply(a1,a2);
	rb = multiply(b1,b2);
	rc = multiply(c1,c2);
	int shif = 1;
	//Left shift match (Step 1, then Step 2):
	for(int i = (2*n)-2; i>=n; i--){
		cout<<"L-shift: "<<shif<<" "<<ra[i] + rb[i] + rc[i]<<'\n';
		shif++;
	} 
	//Right shift match (Step 2, then Step 1):
	shif = 1;
	for(int i = n-2; i>=0; i--){
		cout<<"R-shift: "<<shif<<" "<<ra[i] + rb[i] + rc[i]<<'\n';
		shif++;
	} 
}

//String matching with wildcards ('?')
vector<long long> string_matching(string &s, string &t) { 
	int n = s.size(), m = t.size();
	vector<long long> s1(n), s2(n), s3(n);
	//assign any non zero number for non '?'s
	for(int i = 0; i < n; i++) s1[i] = s[i] == '?' ? 0 : s[i] - 'a' + 1; 
	for(int i = 0; i < n; i++) s2[i] = s1[i] * s1[i];
	for(int i = 0; i < n; i++) s3[i] = s1[i] * s2[i];
	vector<long long> t1(m), t2(m), t3(m);
	for(int i = 0; i < m; i++) t1[i] = t[i] == '?' ? 0 : t[i] - 'a' + 1;
	for(int i = 0; i < m; i++) t2[i] = t1[i] * t1[i];
	for(int i = 0; i < m; i++) t3[i] = t1[i] * t2[i];
	reverse(t1.begin(), t1.end());
	reverse(t2.begin(), t2.end());
	reverse(t3.begin(), t3.end());
	vector<long long> s1t3 = multiply(s1, t3);
	vector<long long> s2t2 = multiply(s2, t2);
	vector<long long> s3t1 = multiply(s3, t1);
	vector<long long> res(n);
	for(int i = 0; i < n; i++) res[i] = s1t3[i] - s2t2[i] * 2 + s3t1[i];
	vector<long long> oc;
	for(int i = m - 1; i < n; i++) if(res[i] == 0) oc.push_back(i - m + 1);
	return oc;
}
