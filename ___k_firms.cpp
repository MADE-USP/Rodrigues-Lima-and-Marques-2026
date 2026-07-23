/******************************************************************************

	K_FIRMS OBJECT EQUATIONS
	----------------------

	Written by Lucca Rodrigues, Made-USP

	Equations that are specific to the capital-good firms objects in the Made-Green model
	are coded below.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
// PART 1: INNOVATION PROCESS----------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================

	EQUATION("_innovation_success") 
	/* UNIT: dummy variable 

	Indicates whether the firm has access to innovation or not in the period (if = 1, firm has access) */

		v[0] = V("_researchers_k_employed") * ( 1 - V("_green_research_subsidie_share") ) * VS(PARENT, "xi") ; // number of researchers the firm employs 	
		v[1] = VS(PARENT, "innovation_easiness"); // parameter indicating how easy it is for firms to innovate

		v[2] = 1-exp(-v[1] * v[0]); // probability in success in innovating

	if ( v[2] < 0 || v[2] > 1 )
	{
		//INTERACT("normal", v[2]);
		LOG("\n -> WARNING: error in bernoulli of %.0lf for firm %.0lf at %.0lf", v[2], V("_id_firm_k"), T );
	}

		v[3] = bernoulli( v[2] ); // bernoulli draw

	RESULT( v[3] )


	EQUATION("_productivity_k_innovation") 
		/* UNIT: number of (capital) goods per worker 

		Productivity given by innovative process for machine production process*/

		v[0] = V("_innovation_success"); // dummy indicating whether the firm was successful in innovating

		v[1] = V("_productivity_k"); // current productivity in machines production

		if(v[0] == 1) // if innovated
		{
			
			v[2] = VS(PARENT, "innovation_alpha");
			v[3] = VS(PARENT, "innovation_beta");
			v[4] = VS(PARENT, "innovation_inf"); // lower beta innovation draw support
			v[5] = VS(PARENT, "innovation_sup"); // upper beta innovation draw support

			v[6] = beta(v[2], v[3]); // beta draw

			v[7] = v[1] * (1 + v[4] + v[6] * (v[5] - v[4]) );
			  
		}
		else // if did not innovate
		{
			v[7] = v[1];
		}

	RESULT(v[7])	
	

	EQUATION("_productivity_k") 
		/* UNIT: number of (capital) goods per worker 

		It represents the number of machines produced by direct workers */

		v[1] = VL("_productivity_k", 1); // previous period productivity
		v[2] = VL("_productivity_k_innovation", 1); // productivity given by innovative effort in previous period


		v[4] = VS(GRANDPARENT, "flag_innovation_separate");
		
		if(v[4] == 0) // if innovative efforts are separate
		{
			v[3] = max(v[1], v[2]); // firm adopts the highest value of productivity
		}

		if(v[4] == 1) // if innovative efforts are not independent
		{
			v[3] = ( V("_productivity_adoption") == 1 ) ? v[2] : v[1];
		}

		v[5] = VS( GRANDPARENT, "flag_damage_function_productivity" );

		if( v[5] == 1)
			v[3] *= VS( GRANDPARENT, "damage_shock_productivity" );		// apply shocks
		
	RESULT(v[3])


	EQUATION("_productivity_c_innovation") 
		/* UNIT: C goods per workers

		Productivity of machines given by innovation process */

		v[0] = V("_innovation_success"); // dummy indicating whether the firm was successful in innovating
		
		v[1] = V("_productivity_c_new"); // current productivity of new machines

		if(v[0] == 1) // if innovated
		{
			
			v[2] = V("innovation_alpha");
			v[3] = V("innovation_beta");
			v[4] = V("innovation_inf"); // lower beta innovation draw support
			v[5] = V("innovation_sup"); // upper beta innovation draw support

			v[6] = beta(v[2], v[3]); // beta draw

			v[7] = v[1] * (1 + v[4] + v[6] * (v[5] - v[4]) );

		}
		else // if did not innovate
		{
			v[7] = v[1];
		}

	RESULT(v[7])	


	EQUATION("_productivity_c_new") 
		/* UNIT: C goods per worker 

		It represents the number of goods produced if the new machine is used by the maximum number of workers */

		v[1] = VL("_productivity_c_new", 1); // previous period productivity
		v[2] = VL("_productivity_c_innovation", 1); // productivity given by innovative effort in previous period


	v	[4] = VS(GRANDPARENT, "flag_innovation_separate");
		
		if(v[4] == 0) // if innovative efforts are separate
		{
			v[3] = max(v[1], v[2]); // firm adopts the highest value of productivity
		}

		if(v[4] == 1) // if innovative efforts are not independent
		{
			v[3] = ( V("_productivity_adoption") == 1 ) ? v[2] : v[1];
		}
		
		v[5] = VS( GRANDPARENT, "flag_damage_function_productivity" );

		if( v[5] == 1)
			v[3] *= VS( GRANDPARENT, "damage_shock_productivity" );		// apply shocks

	RESULT( v[3] )


	EQUATION("_productivity_c_new_growth")
		/* UNIT: rate

		Growth rate of productivity of new machines */

		v[0] = V("_productivity_c_new");
		v[1] = VL("_productivity_c_new", 1);

		v[2] = (v[1] == 0)? 0: ( v[0] / v[1] ) - 1;

	RESULT(v[2])


	EQUATION("_productivity_k_growth")
		/* UNIT: rate

		Growth rate of productivity in K sector */

		v[0] = V("_productivity_k");
		v[1] = VL("_productivity_k", 1);

		v[2] = (v[1] == 0)? 0: ( v[0] / v[1] ) - 1;

	RESULT(v[2])

	EQUATION("_green_innovation_success") 
	/* UNIT: dummy variable 

	Indicates whether the firm has access to innovation or not in the period (if = 1, firm has access) */

		v[0] = V("_researchers_k_employed") * VS(PARENT, "xi"); // number of researchers the firm employs
		v[1] = VS(PARENT, "green_innovation_easiness"); // parameter indicating how easy it is for firms to innovate

		v[2] = 1-exp(-v[1] * v[0]); // probability in success in innovating

	if ( v[2] < 0 || v[2] > 1 )
	{
		//INTERACT("green", v[2]);
		LOG("\n -> WARNING: error in bernoulli of %.0lf for firm %.0lf at %.0lf", v[2], V("_id_firm_k"), T );
	}

		v[3] = bernoulli( v[2] ); // bernoulli draw

	RESULT( v[3] )	


	EQUATION("_green_productivity_c_innovation") 
		/* UNIT: Emissions per C good produced

		Productivity of machines given by innovation process */

		v[0] = V("_green_innovation_success"); // dummy indicating whether the firm was successful in innovating

		v[1] = V("_green_productivity_c_new"); // current productivity of new machines 

		if(v[0] == 1) // if innovated
		{
			
			v[2] = VS(PARENT, "green_innovation_alpha");
			v[3] = VS(PARENT, "green_innovation_beta");
			v[4] = VS(PARENT, "green_innovation_inf"); // lower beta innovation draw support
			v[5] = VS(PARENT, "green_innovation_sup"); // upper beta innovation draw support

			v[6] = beta(v[2], v[3]); // beta draw

			v[7] = v[1] * (1 - v[4] - v[6] * ( v[5] - v[4]) ); //minus innovation change because we want to reduce the emissions coefficient

		}
		else // if did not innovate
		{
			v[7] = v[1];
		}

		if(VS(government2, "flag_green_lab") == 1){ //if the green lab is working
			v[8] = min(VLS(government2, "green_productivity_g",1), v[7]); //can adopt green technology without cost
		}
		else{
			v[8] = v[7];
		}

	RESULT(v[8])


	EQUATION("_green_productivity_k_innovation") 
		/* UNIT: Emissions per K good produced

		Productivity given by innovative process for machine production process*/

		v[0] = V("_green_innovation_success"); // dummy indicating whether the firm was successful in innovating

		v[1] = V("_green_productivity_k"); // current productivity in machines production

		if(v[0] == 1) // if innovated
		{

			v[2] = VS(PARENT, "green_innovation_alpha");
			v[3] = VS(PARENT, "green_innovation_beta");
			v[4] = VS(PARENT, "green_innovation_inf"); // lower beta innovation draw support
			v[5] = VS(PARENT, "green_innovation_sup"); // upper beta innovation draw support

			v[6] = beta(v[2], v[3]); // beta draw

			v[7] = v[1] * (1 - v[4] - v[6] * (v[5] - v[4]) ); //minus innovation change because we want to reduce the emissions coefficient 
			  
		}
		else // if did not innovate
		{
			v[7] = v[1];
		}

		if(VS(government2, "flag_green_lab") == 1){ //if the green lab is working
			v[8] = min(VLS(government2, "green_productivity_g",1), v[7]); //can adopt green technology without cost
		}
		else{
			v[8] = v[7];
		}

	RESULT(v[8])


	EQUATION("_green_productivity_c_new") 
		/* UNIT: Emissions per C good produced

		It represents the tons of Co2 emissions per goods produced if the new machine is used by the maximum number of workers */

		v[1] = VL("_green_productivity_c_new", 1); // previous period emissions coefficient
		v[2] = VL("_green_productivity_c_innovation", 1); // emissions coefficient given by innovative effort in previous period

		v[4] = VS(GRANDPARENT, "flag_innovation_separate");
		
		if(v[4] == 0) // if innovative efforts are separate
		{
			v[3] = min(v[1], v[2]); // firm adopts the lowest emissions coef
		}

		if(v[4] == 1) // if innovative efforts are not independent
		{
			v[3] = ( V("_productivity_adoption") == 1 ) ? v[2] : v[1];
		}

	RESULT( v[3] )	


	EQUATION("_green_productivity_k") 
		/* UNIT: Emissions per K good produced

		It represents the number of machines produced by direct workers */

		v[1] = VL("_green_productivity_k", 1); // previous period productivity
		v[2] = VL("_green_productivity_k_innovation", 1); // productivity given by innovative effort in previous period

		v[4] = VS(GRANDPARENT, "flag_innovation_separate");
		
		if(v[4] == 0) // if innovative efforts are separate
		{
			v[3] = min(v[1], v[2]); // firm adopts the lowest emissions coef
		}

		if(v[4] == 1) // if innovative efforts are not independent
		{
			v[3] = ( V("_productivity_adoption") == 1 ) ? v[2] : v[1];
		}

	RESULT(v[3])

	EQUATION("_imitation_k")
		/*UNIT: no unit
		
		Update innovation parameters if imitation is more sucessfull variables*/

		// imitation process (success probability)
		
		v[0] = 1 - exp( - VS( PARENT, "imitation_easiness" ) * (1-VS(PARENT, "xi")) *  V("_researchers_k_employed") );

		v[11] = ( 1 + VS( PARENT, "mark_up_k" ) ) * ( VL("_wage_k", 1) / VL("_productivity_k_innovation", 1) + ( VS( government2, "CO2_tax" ) * VL("_green_productivity_k_innovation", 1) ) );

		v[12] =  VLS(c_sector2, "workers_wage_c_avg", 1) / VL("_productivity_c_innovation", 1) + ( VS( government2, "CO2_tax" ) * VL("_green_productivity_c_innovation", 1) ) ;

		v[7] = V( "_productivity_c_new" );	// get imitated firm productivities
		v[8] = V( "_green_productivity_c_new" );
		v[9] = V( "_productivity_k" );
		v[10] = V( "_green_productivity_k" );

		if ( bernoulli( v[0] ) == 1 )						// imitation succeeded?
			{
				v[1] = VLS(c_sector2, "workers_wage_c_avg", 1) / VL("_productivity_c_new", 1) + VS( government2, "CO2_tax" ) * VL("_green_productivity_c_new", 1);	

				k = VS( PARENT, "total_k_firms" );						// number of firms in sector 1
				dblVecT imiProb( k );						// vector for tech distance

				v[2] = i = 0;								// inverse distance/firm accum.
				CYCLES( PARENT, cur, "k_firms" )	// 1st run: abs. inv. distance
					if ( cur == THIS )
						imiProb[ i++ ] = 0;					// can't self-imitate
					else
					{
						// price and operating cost of firm candidate for imitation
						v[3] = ( 1 + VS( PARENT, "mark_up_k" ) ) * ( VLS(cur, "_wage_k", 1) / VLS( cur, "_productivity_k", 1) +
									 ( VS( government2, "CO2_tax" ) * VLS( cur, "_green_productivity_k", 1) ));

						v[4] = VLS(c_sector2, "workers_wage_c_avg", 1) / VLS( cur, "_productivity_c_new",1 ) +
					   				( VS( government2, "CO2_tax" ) * VLS( cur, "_green_productivity_c_new", 1 ) );

						// euclidian distance in 2-dimensional mean-standardized space
						v[5] = sqrt( pow(  (v[3] - VLS(cur, "_price_k", 1) ) / VLS( PARENT, "price_k_avg", 1 ), 2 ) +
						 pow( ( v[4] - v[1] ) / VLS( c_sector2, "cost_unit_production_c_avg", 1 ), 2 ) );

						v[2] += imiProb[ i++ ] = ( v[5] > 0 ) ? 1 / v[5] : 0;
					}

					if ( v[2] > 0 )
					{
						v[6] = i = 0;							// probabilities/firm accum.
						CYCLES( PARENT, cur, "k_firms" )			// 2nd run: cumulative imi. prob.
						{
							v[6] += imiProb[ i ] / v[2];		// normalize to add up to 1
							imiProb[ i++ ] = v[6];
						}

						// draw a firm to imitate according to the distance probabilities
						j = upper_bound( imiProb.begin( ), imiProb.end( ), RND ) - imiProb.begin( );

						if ( j < k )							// at least one firm reachable?
							{
							cur = TSEARCHS( PARENT, "k_firms", j + 1 );// get pointer to firm

							v[7] = VLS( cur, "_productivity_c_new", 1 );	// get imitated firm productivities
							v[8] = VLS( cur, "_green_productivity_c_new", 1 );
							v[9] = VLS( cur, "_productivity_k", 1 );
							v[10] = VLS( cur, "_green_productivity_k", 1 );

							// price and operating cost of new machine
							v[11] = ( 1 + VS( PARENT, "mark_up_k" ) ) * ( VL("_wage_k", 1) / v[9] + ( VS( government2, "CO2_tax" ) * v[10] ) );
							v[12] = VLS(c_sector2, "workers_wage_c_avg", 1) / v[7] + ( VS( government2, "CO2_tax" ) * v[8] ) ;
							}
					}	
			}

			v[13] =  ( 1 + VS( PARENT, "mark_up_k" ) ) * ( VL("_wage_k", 1) / VL("_productivity_k_innovation", 1) + ( VS( government2, "CO2_tax" ) * VL("_green_productivity_k_innovation", 1) ) );
			
			v[14] = VLS(c_sector2, "workers_wage_c_avg", 1) / VL("_productivity_c_innovation", 1) + ( VS( government2, "CO2_tax" ) * VL("_green_productivity_c_innovation", 1) ) ;

			if ( v[11] + v[12] < v[13] + v[14])
				{
					WRITE( "_productivity_c_innovation", v[7]);
					WRITE( "_green_productivity_c_innovation", v[8]);
					WRITE( "_productivity_k_innovation", v[9]);
					WRITE( "_green_productivity_k_innovation", v[10]);
				}
			
			

		RESULT( v[0] )


	EQUATION("_productivity_adoption")
		/* UNIT: dummy variable

		Indicates whether the firm will adopt the new technologies in case the innovative efforts are not independent. This variable takes 1 if yes and 0 otherwise.

		It is only relevant in case adoption of new A and B is not independent. */

		if(VS(GRANDPARENT, "flag_innovation_separate") == 0) END_EQUATION( 0 ); // if adoption of AB is independent - more productive technology will always be adopted (no trade-off)

		if(VL("_innovation_success", 1) == 0 && VL("_imitation_k", 1) == 0 ) END_EQUATION( 0 ); // no new technology to be adopted


		v[7] = VS(PARENT, "mark_up_k");
		v[8] = VS( government2, "CO2_tax");
		v[9] = VL("_wage_k", 1); 
		v[99] = VLS(c_sector2, "workers_wage_c_avg", 1); 

		v[10] = VL("_productivity_c_new", 1);
		v[11] = VL("_productivity_k", 1);
		v[12] = VL("_green_productivity_c_new", 1);
		v[13] = VL("_green_productivity_k", 1);
		
		v[20] = VL("_productivity_c_innovation", 1);
		v[21] = VL("_productivity_k_innovation", 1);
		v[22] = VL("_green_productivity_c_innovation", 1);
		v[23] = VL("_green_productivity_k_innovation", 1);

		v[40] = 0;

		if( v[22] < v[12] || v[23] < v[13] ){
			//v[40] = VLS(government2, "green_price_subsidie", 1);
		}

		// otherwise, there is a trade-off evaluation

		// using old technology

			v[15] = v[99] / v[10] + v[8] * v[12] ; //cost for C firm

			v[16] = (1 + v[7]) * ( v[9] / v[11] + v[8] * v[13]) ; // price using old technology

		// using new technology

			v[25] = v[99] / v[20] + v[8] * v[22] ; //cost for C firm

			v[2] =  (1 + v[7]) *  ( v[9] / v[21] + v[8] * v[23]); // price using new technology 

			v[26] = max(  v[2] * 0.5,  
						  v[2] - v[40] ); //price subsidie

			v[3] = v[2] - v[26];
			//WRITE( "_green_price_subsidie_k", v[3]);

		// decision rule

			if( v[15] + v[16] < v[25] + v[26] )  // if cost + price with old machine is lower than with new
			{
				v[30] = 0; // new technology is not adopted
			}
			else
			{
				v[30] = 1; // new technology is adopted
			}
	

	RESULT( v[30] )	

	//EQUATION_DUMMY( "_green_price_subsidie_k", "_productivity_adoption")



//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
// PART 2: PRODUCTION PROCESS----------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================


	EQUATION("_production_desired_k") 
		/* UNIT: number of capital goods

		Determines the number of capital goods the firm wants to produced in the period according to the demand for investment goods received from the C firms. */

		v[0] = V("_orders_k"); // total orders received from the consumption goods firm
		
	RESULT( v[0] )


	EQUATION("_production_k") 
		/* UNIT: number of capital goods

		Determines the total production of machines by the firm */

		v[0] = V("_production_desired_k"); // number of goods ordered by C firms

		v[1] = V("_workers_k_employed"); // number of direct workers employed at the firm
		
		v[2] = V("_productivity_k") ; // labor productivity at the k goods production
		v[3] = v[1] * v[2];  // maximum number of capital goods that can be produced by the hired direct workers

		v[5] = min(v[0], v[3]); // number of goods produced by the firm
		v[6] = max(v[5], 0); // production cannot be lower than zero (just to be sure)

	RESULT(v[6])


	EQUATION("_production_canceled_k") 
		/* UNIT: number of capital goods

		Determines the total production of machines by the firm */

		v[0] = V("_production_desired_k"); // number of goods ordered by C firms

		v[1] = V("_production_k"); 

	RESULT(v[0] - v[1])


	EQUATION( "_clients_new_k" )
		/* UNIT: number of clients (c firms)
		
		Number of new client firms in the period. Also creates the client-supplier connecting objects.*/

		V("_clients_historical_k");

		firmMapT firms = V_EXTS( GRANDPARENT, countryE, firmCmap );// list with all firms

		k = firms.size( );								// number of firms in goods sector

		j = max( 1, ceil( k / VS( GRANDPARENT, "init_total_k_firms" ) ) ) ;	// firm fair share

		// build vector of all target firms (not yet clients)
		vector < firmPairT > targets( firms.begin( ), firms.end( ) );
		// draw new clients from target list, updating the list after each draw
		for ( k = 0; j > 0 && targets.size( ) > 0; --j, ++k )
		{
			h = uniform_int( 0, targets.size( ) - 1 );	// draw a index in the list
			firmPairT client = targets[ h ];			// retrieve drawn map pair
			targets.erase( targets.begin( ) + h );		// remove drawn firm from list

			// create the brochure/client interconnected objects
			send_brochure( THIS, client.second );

		}

	RESULT( k )


	EQUATION( "_clients_historical_k" )
		/* UNIT: number of clients (c firms)
		
		Number of historical client firms. Also removes old, non-buying clients.
		*/

		i = 0;											// client counter
		CYCLE_SAFE( cur, "clients" )						// remove old goods sector clients
		{
			if ( VS( cur, "__tSel" ) < T )			// last selection is old?
			{
				DELETE( SHOOKS( cur ) );				// remove supplier brochure entry
				DELETE( cur );							// remove client entry
			}
			else
			++i;	
		}

	RESULT( i )
	

	EQUATION( "_orders_k" )
		/* UNIT: number of capital goods

		Potential demand (orders) received by a firm in capital-good sector*/
		
		VS(c_sector2, "investment_demand_c");							// make sure all orders are sent

		v[0] = SUM_CND( "__nOrd", "__tOrd", "==", T );

		v[1] = VL("_green_k",1) == 1? v[0] + VS(government2, "green_public_procurement_k") : v[0];
		
	RESULT( v[1] )	


	EQUATION( "_market_share_k" )
		/* UNIT: share

		Market share of K firm in the K sector*/
		
		v[0] =VS(PARENT, "production_k") == 0? 0: V("_production_k") / VS(PARENT, "production_k");
		
	RESULT( v[0] )	

//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
//-------------- PART 3: LABOR MARKET EQUATIONS ---------------------------------------------------------------------------------------------------===================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================

	EQUATION("_wage_k")
		/*UNIT: monetary units
		
		Wage paid by a firm K to direct workers*/

		v[1] = VS( hh_sector2, "psi_inflation" );								// inflation adjust. parameter
		v[2] = VS( hh_sector2, "psi_productivity" );								// individual prod. adjust. param.
		v[3] = VS( hh_sector2, "psi_productivity_avg" );								// general. prod. adjust. parameter
		v[4] = max(VLS( GRANDPARENT, "inflation", 1 ), 0);								// current inflation
		v[5] = max(VL( "_productivity_k_growth", 1 ), 0);							//individual workers productivity var.
		v[6] = max(VLS( GRANDPARENT, "productivity_change", 1 ), 0);					// general productivity variat.
		v[7] = VL("wage", 1) * ( 1 + v[1] * v[4] + v[2] * v[5] + v[3] * v[6] ); 	//workers desired wage level 

		v[8] = VL("_wage_k", 1); //desired wage firms
		v[9] = VLS(hh_sector2, "unemployment_rate", 1);

		v[0] = (1 - VS(hh_sector2, "wage_sensitivity_hh")) * v[8] + VS(hh_sector2, "wage_sensitivity_hh") * v[7];

	RESULT( v[0] ) 


	EQUATION("_research_wage_k")
		/*UNIT: monetary units
		
		Wage for researchers with a premium from direct wage*/

		v[0] = V("_wage_k") * (1 + VS( hh_sector2, "wage_premium"));
	
	RESULT( v[0]) 


	EQUATION("_managers_wage_k")
		/*UNIT: monetary units
		
		Wage for managers with a premium from direct wage*/

		v[0] = V("_wage_k") * (1 + VS( hh_sector2, "wage_premium"));
	
	RESULT( v[0]) 


	EQUATION("_researchers_k_demand") 
		/* UNIT: number of researchers

		Demand for researchers by the firm: depends on the previous period revenue */

		v[0] = V("_research_budget_k") / V("_research_wage_k");

	RESULT( v[0] )		

	EQUATION("_researchers_k_employed") 
		/* UNIT: number of researchers

		Number of researchers hired at the firm */

		v[2] = V("_researchers_k_demand") * VS(hh_sector2, "labor_restriction"); //potential shortage is proportional to firm initial demand

	RESULT( v[2] )
		
	EQUATION("_workers_k_demand") 
		/* UNIT: number of workers

		Sets the demand for workers by the capital goods firm according to the level of production in the current period*/

		v[0] = V("_production_desired_k"); // desired production level (depends on the demand for K goods received from C firms)
		
		v[1] = V("_productivity_k"); // labor productivity in the production of K goods 
		
		v[2] = v[0] / v[1]; // number of workers needed to produce the desired quantity of machines

	RESULT( round(v[2]) )


	EQUATION("_managers_k_demand") 
		/* UNIT: number of workers

		Sets the demand for managers by the capital goods firm according to the level of production in the current period*/

		v[0] = V("_workers_k_demand") * VS(PARENT, "manager_per_worker_k"); 

	RESULT( round(v[0]) )


	EQUATION("_workers_k_employed") 
		/* UNIT: number of workers

		Sets the employed workers for K firm in the period */

		v[2] = V("_workers_k_demand") * VS(hh_sector2, "labor_restriction"); //potential shortage is proportional to firm initial demand
		
	RESULT( round (v[2]) )

	EQUATION("_managers_k_employed") 
		/* UNIT: number of workers

		Sets the employed managers for K firm in the period */

		v[2] = V("_managers_k_demand") * VS(hh_sector2, "labor_restriction"); //potential shortage is proportional to firm initial demand
		
	RESULT( round(v[2]) )

//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
// PART 4: COSTS AND PRICE ------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
	
	EQUATION("_cost_unit_production_k") 
		/* UNIT: monetary units

		Total unit costs of production. */

			v[1] = V("_wage_k"); 
			v[2] = V("_managers_wage_k") * VS(PARENT, "manager_per_worker_k"); 
			v[3] = V("_productivity_k"); // labor productivity in capital goods sector
			v[4] = V( "_green_productivity_k" );
			v[5] = VS( government2, "CO2_tax" );
		
			v[6] = (v[1] + v[2]) / v[3]; // costs with workers
			v[7] = v[4] * v[5]; // costs with emissions

			v[10] = v[6] + v[7] ; // expected unit costs

	RESULT( v[10] )


	EQUATION("_price_k")
		/* UNIT: monetary units

		Firms set prices according to a mark-up applied to unit costs */

		v[0] = VS(PARENT, "mark_up_k"); // mark-up rate
		v[1] = V("_cost_unit_production_k"); // unit costs
		
		
		if( V("_green_k") == 1 ) v[2] = max(
											( 1 + v[0] ) * v[1] * 0.5,  
											( 1 + v[0] ) * v[1] - VLS(government2, "green_price_subsidie", 1)); 
											// price of the capital good WITH SUBSIDIE capped at 50% discount
		else v[2] = ( 1 + v[0] ) * v[1]; // price of the capital good

		v[3] = ( 1 + v[0] ) * v[1] - v[2];
		WRITE( "_green_price_subsidie_k", v[3])

		//v[2] = ( 1 + v[0] ) * v[1] - V( "_green_price_subsidie_k" );

	RESULT( v[2] )

	EQUATION_DUMMY( "_green_price_subsidie_k", "_price_k")


	EQUATION("_research_budget_k")
		/* UNIT: monetary units
		
		 Available resources for research*/

		 v[0] = VL("_profits_net_k", 1) * VS(PARENT, "nu") + VS(government2, "green_research_subsidie");

		 //v[0] = VL("_green_k",1) == 1? VL("_profits_net_k", 1) * VS(PARENT, "nu") + VS(government2, "green_research_subsidie") : VL("_profits_net_k", 1) * VS(PARENT, "nu");

		 v[1] = max(v[0], 0);
		
		 WRITE("_green_research_subsidie", VLS(government2, "green_research_subsidie", 1));

		 v[2] = v[0] ==0? 0: max(0,min(1,VS(government2, "green_research_subsidie") / v[0]));

		 WRITE("_green_research_subsidie_share", v[2] );

	RESULT(v[1])

	EQUATION_DUMMY( "_green_research_subsidie", "_research_budget_k")

	EQUATION_DUMMY( "_green_research_subsidie_share", "_research_budget_k")


	EQUATION("_research_cost_k")
		/* UNIT: monetary units

		Resources available for financing research (considering resources that will be used to hire indirect workers, already rounded down) */
	
		v[0] = V("_researchers_k_employed");
		v[1] = V("_research_wage_k")

	RESULT( v[0] * v[1] )


	EQUATION("_cost_workers_k")
		/* UNIT: monetary units

		Cost with employed workers in the capital goods sector  */
	
		v[0] = V("_workers_k_employed");
		v[10] = V("_wage_k");

	RESULT( v[0] * v[10] )	

	EQUATION("_cost_managers_k")
		/* UNIT: monetary units

		Cost with employed workers in the capital goods sector  */
	
		v[0] = V("_managers_k_employed");
		v[10] = V("_managers_wage_k");

	RESULT( v[0] * v[10] )


	EQUATION("_cost_production_k") 
		/* UNIT: monetary units

		Total production costs with wages (R&D costs are not considered) */ 


		v[1] = V("_cost_workers_k");
		v[2] = V("_cost_managers_k");
		// V("_research_cost_k") cost with researchers (it is paid with the previous period revenue - not considered here)
		v[3] = VS(government2, "CO2_tax") * V("_emissions_k"); //cost with emissions

		v[4] = v[1] + v[2] + v[3]; 

	RESULT( v[4] )


	EQUATION("_sales_k_nominal") 
		/* UNIT: monetary units

		Revenue of the capital goods firm */
		
		v[0] = V("_production_k"); 

		v[1] = V("_price_k") + V( "_green_price_subsidie_k"); // price of the capital good (WITH SUBSIDIE when applicable)
		
		v[2] = v[0] * v[1];

	RESULT( v[2] )	


	EQUATION("_taxes_sales_paid_k")
		/* UNIT: monetary units
		
		Taxes paid by firm sales by the capital goods firm */

		v[0] = V("_sales_k_nominal") * VS(government2, "tax_rate_sales") ;

	RESULT( v[0] )	

	EQUATION("_profits_gross_k") 
		/* UNIT: monetary units

		Gross profits: nominal sales discounted by costs and taxes */ 
	
		v[0] = V("_sales_k_nominal"); 
		v[1] = V("_cost_production_k"); 
		v[2] = V("_taxes_sales_paid_k"); 
	
		v[4] = v[0] - v[1] - v[2] ;
	
	RESULT( v[4] )


	EQUATION("_profits_net_k") 
		/* UNIT: monetary units

		Net profits of capital goods firms*/

		v[0] = V("_profits_gross_k"); // gross profits (already discounted by the tax rate )

		v[1] = VL("_deposits_k", 1);

		v[3] = v[1] > 0 ? 0 : VLS(financial2, "interest_rate_loan", 1);

		v[4] = v[1] * ( v[3] ); // amount of interest earned in the period

	RESULT( v[0] + v[4] )	


	EQUATION("_deposits_k") 
		/* UNIT: monetary units

		Deposits of capital goods firms*/

		v[0] = V("_profits_net_k") - V("_profits_distributed_k"); // net profits (already discounted by the tax rate and distributed dividends)

		v[1] = VL("_deposits_k", 1);

		v[2] = V("_research_cost_k") - VLS(government2, "green_research_subsidie", 1);

		v[4] = v[0] + v[1] - v[2] ; 

		if(v[4] < 0) {

			WRITE("_loan_k", -v[4]);

			LOG("\n -> WARNING: deposits for capital goods firm negative (used loan) in time period %.0lf", T );
		}
	RESULT( v[4] )

	EQUATION_DUMMY("_loan_k", "_deposits_k")

	EQUATION("_profits_distributed_k") 
		/* UNIT: monetary units

		Distributed profits */

		v[1] = V("_profits_net_k") ; // profits available for distributing  

		v[2] = VS(PARENT, "dividends_target_k");

		v[3] = v[1] * v[2]; //target value for distributed dividends

		v[4] = max( v[3], 0 ); // amount of profits to be distributed (cannot be negative)

		v[11] = v[4] / VS(GRANDPARENT, "capitalists_per_firm");

		WRITE("_dividends_k", v[11]);
	 	
	RESULT( v[4] )

 
//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
// PART 5: ENVIRONMENTAL EQUATIONS-----------------------------------------------------------------------------------------------------------------===================================================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================


	EQUATION( "_emissions_k" )
		/* UNIT: tons of CO2 
		
		Carbon emissions produced by firm in capital-good sector */

		v[0] = V( "_production_k" ) * V( "_green_productivity_k" ); 


	RESULT( v[0] )

	EQUATION_DUMMY("_dividends_k", "_profits_distributed_k")

	EQUATION_DUMMY("_wage_k_growth", "_wage_k")

	EQUATION_DUMMY( "_green_k", "green_k")






