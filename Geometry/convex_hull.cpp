vector <Point> calculateHull(vector <Point> &p, int n){ //Calculo del Convex Hull
	if (n <= 2) return p;
	vector<Point> hull;
	int tam = 0;
	sort(p.begin(),p.end());
	fore(t,0,2){
		fore(i,0,n){
			while((int)hull.size()-tam >= 2){
				Point p1 = hull[(int)hull.size()-2];
				Point p2 = hull[(int)hull.size()-1];
				//Producto cruz: P1 es el ancla
				//agregar (<=) si tambien se quieren incluir los puntos colineales, sino solo (<)
				if(p1.cross(p2, p[i]) <= 0) break; 
				hull.pop_back();
			}
			hull.push_back(p[i]);
		}
		hull.pop_back();
		tam = (int)hull.size();
		reverse(p.begin(),p.end());
	}
	return hull;
}