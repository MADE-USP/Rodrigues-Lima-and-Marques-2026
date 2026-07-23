/******************************************************************************

	C_FIRMS OBJECT EQUATIONS
	----------------------

	Written by Lucca Rodrigues, Made-USP

	Equations that are specific to the consumption-good firms objects in the Made-Green model
	are coded below.

 ******************************************************************************/

// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// PART 1: EXPECTATIONS AND DESIRED PRODUCTION--------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================

	EQUATION("_sales_expectations_c")  
		/* UNIT: number of consumption goods

		Computes the firms' sales/demand expectations for the current period, which will be used for its production decisions */


			int _i;
			int _j; 

			for(_i = 1, _j = 5, v[30] = 0, v[31] =0; _i < _j; _i++){
						

				v[0] = VL("_demand_c_real", _i); // demand for firm's products in the period
							
					// weight given to past value:
						if(_i == 1) v[1] = VS(PARENT, "sensitivity_demand_sales1");  
						if(_i == 2) v[1] = VS(PARENT, "sensitivity_demand_sales2");
						if(_i == 3) v[1] = VS(PARENT, "sensitivity_demand_sales3");
						if(_i == 4) v[1] = VS(PARENT, "sensitivity_demand_sales4");

						v[30] += v[0] * v[1]; // updates sales * weight

						v[31] += v[1]; // updates counter of weights
			}
			
			//v[20] = (v[31] == 0) ?( V("_capacity_ut_desired_c") * V("_capital_stock_c") ) : (v[30] / v[31]);
		
		
		v[21] = max(v[30] / v[31], 0);

	RESULT( v[21] )

 
	EQUATION("_production_desired_c") 
		/* UNIT: number of consumption goods

		Desired level of production for the current period: already limited by the firm's capital stock */

		v[0] = V("_sales_expectations_c"); // sales expectations for the current period
		v[1] = VS(PARENT,"inventories_desired_share"); // share of desired inventories with respect to expected sales
		v[2] = VL("_inventories_c", 1); // past level of inventories

		v[3] = SUM("__capital_vintage"); // production level at full capacity utilization in the current period

		v[4] = v[0] * (1 + v[1]) - v[2]; // desired production level

		v[5] = min(v[4], v[3]); // the firm will produce the minimum between its desired production and the production level at full capacity utilization

		v[6] = max(v[5], 0); // there is no negative desired production: if the current inventories are larger than expected sales+ desired inventories, the firm does not produce in the period

		//in the current period, firms desired production is limited by capital stock
		//but not desired capacity utilization, to be able to demand more capital to the next period

		v[7] = max(v[4], 0);

		WRITE("_capacity_ut_desired_c", v[7]/v[3] )
		
	RESULT( v[6] )


	/*EQUATION("_capacity_ut_desired_c")
		/* UNIT: rate
		
		Desired capacity utilization of a firm in consumption-good sector

		v[1] = VL("_capital_stock_c",1)  * (1 - VS(PARENT, "delta"));

		v[0] =  V("_production_desired_c") /  v[1];

	RESULT( v[0] )	*/


	EQUATION("_cost_production_expected_c") 
		/* UNIT: monetary units

		Computes the labor costs that the firm expects to face if it produces the level of production determined before financial restrictions are taken into account*/

		v[0] = V("_cost_emissions_expected_c"); // expected costs with emissions
		v[1] = V("_cost_workers_expected_c"); // expected costs with workers
		v[2] = V("_cost_managers_expected_c"); // expected costs with managers

		v[3] = v[0] + v[1] + v[2]; // total expected labor costs

	RESULT( v[3] )
	


	EQUATION("_cost_workers_expected_c") 
		/* UNIT: monetary units

		Indicates the expected cost with workers, before the financial restrictions upon the firm are considered */

		v[0] = V("_wage_c"); // wage that the firm pays to its workers 
		v[1] = V("_workers_c_desired"); // demand for managers

		v[2] = v[0] * v[1]; 

	RESULT( v[2] )	

	EQUATION("_cost_managers_expected_c") 
		/* UNIT: monetary units

		Indicates the expected cost with managers, before the financial restrictions upon the firm are considered */

		v[0] = V("_managers_wage_c"); // wage that the firm pays to its workers 
		v[1] = V("_managers_c_desired"); // demand for managers

		v[2] = v[0] * v[1]; 

	RESULT( v[2] )	


	EQUATION("_cost_emissions_expected_c") 
		/* UNIT: monetary units

		Indicates the expected cost with emissions, before the financial restrictions upon the firm are considered */

		v[0] = VS(government2, "CO2_tax"); // cost of emissions
		v[1] = V("_emissions_c_expected"); // emissios at desired level of production

		v[2] = v[0] * v[1]; 

	RESULT( v[2] )


	EQUATION("_cost_unit_production_c_expected") 
		/* UNIT: monetary units

		Expected unit costs before hiring and effective demand.  */

		v[1] = V("_cost_production_expected_c"); // demand for direct workers has to be updated before

		v[2] = V("_production_desired_c"); // production at desired capacity utilization rate

		if(v[2] > 0)
		{
			v[11] = v[1] / v[2]; // unit costs at desired capacity utilization rate
		}
		else
		{
			v[11] = 0; // there is no cost if production is zero 
		} 

	RESULT( v[11] )	

	EQUATION("_mark_up_desired_c") 
		/* UNIT: ratio

		Desired mark-up ratio taking into consideration competition in C market */

		v[0] = VL("_mark_up_desired_c", 1);

		v[1] = VL("_market_share_c", 1) ; // market share of the firm in the previous period
		v[2] = VL("_market_share_c", 2);

		v[3] = ROUND(v[2], 0, 0.001) == 0? 0: (v[1] / v[2]) - 1; // change in market share

		v[4] = VS(PARENT, "sensitivity_mark_up_market_share_c"); 

		v[5] = v[4] * v[3]; // change in the mark up  

		v[6] = v[0] * (1 + v[5]);
	
		v[7] = max(v[6], 0);

	RESULT( v[7] )		


	EQUATION("_cost_investment_expected_c") 
		/* UNIT: monetary units

		Cost of desired investment by the firm */

		V("_supplier");

		cur = PARENTS( SHOOKS( SHOOK ) ) ;// hook current supplier

		v[0] = max(V("_investment_desired_c"), 0); // number of capital goods that the firm wishes to acquire
		v[1] = VS(cur, "_price_k"); // price of capital good
		
		v[2] = v[0] * v[1];

	RESULT( v[2] )

// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// PART 2: LABOR MARKET --------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================

	EQUATION("_wage_c")
		/*UNIT: monetary units
		
		Wage paid by a firm K to direct workers*/

		V( "_supplier" );
		object *suppl; 
		suppl = PARENTS( SHOOKS( SHOOK ) ) ;// current supplier

		v[1] = VS( hh_sector2, "psi_inflation" );								// inflation adjust. parameter
		v[2] = VS( hh_sector2, "psi_productivity" );								// individual prod. adjust. param.
		v[3] = VS( hh_sector2, "psi_productivity_avg" );								// general. prod. adjust. parameter
		v[4] = max(VLS( GRANDPARENT, "inflation", 1 ), 0);								// current inflation
		v[5] = max(VLS( suppl, "_productivity_c_new_growth", 1 ), 0);							//individual workers productivity var.
		v[6] = max(VLS( GRANDPARENT, "productivity_change", 1 ), 0);					// general productivity variat.
		v[7] = VL("wage", 1) * ( 1 + v[1] * v[4] + v[2] * v[5] + v[3] * v[6] ); 	//workers desired wage level 

		v[8] = VL("_wage_c", 1); //desired wage firms
		v[9] = VLS(hh_sector2, "unemployment_rate", 1);

		v[0] = (1 - VS(hh_sector2, "wage_sensitivity_hh")) * v[8] + VS(hh_sector2, "wage_sensitivity_hh") * v[7];


	RESULT( v[0] )

	EQUATION("_managers_wage_c")
		/*UNIT: monetary units
		
		Wage for managers with a premium from direct wage*/

		v[0] = V("_wage_c") * (1 + VS( hh_sector2, "wage_premium"));
	
	RESULT( v[0]) 


	EQUATION("_workers_c_desired") 
		/* UNIT: number of workers

		Sets the demand for workers by the consumption goods sector according to the level of production desired by the firm before taking into account the financial restrictions upon the firm. This value is updated in the open positions function later, wherein financial restrictions are considered.*/

		v[0] = V("_production_desired_c"); // number of goods that the firm wants to produce

		v[11] = 0; // sets counter of produced goods to zero
		v[12] = 0; // sets counter of number of workers needed to zero

		v[15] = 0; // sets counter of emissions to zero

		SORT("vintage", "__productivity_vintage", "DOWN"); // sorts the machines down (from more to less productive)
	
		CYCLE(cur, "vintage") // cycle for the sorted machines
		{

			v[19] = VS(cur, "__green_productivity_vintage");
	
			v[20] = VS(cur, "__worker_vintage_max"); // maximum number of workers employed at the vintage (to produce the maximum quantity) - considering all its machines
				
			v[21] = VS(cur,"__productivity_vintage"); // productivity of the the vintage
				
			v[22] = VS(cur, "__capital_vintage"); // maximum number of goods that can be produced by all the vintage's machines at full capacity utilization

			v[23] = v[0] - v[11]; // number of goods that still need to be produced to reach the desired production level

			// current production
				if(v[23] > 0 && v[21] > 0) // if the number of goods produced so far is smaller than the number of goods the firm wants to produce -> the vintage will be used
				{
					if(v[23] >= v[22]) // if the number of goods that still need to be produced is larger than the max number of goods the vintage can produce, it will be entirely utilized
					{
						v[11] += v[22]; // adds the number of goods produced by the firm to the counter of goods
						v[12] += v[20]; // adds the max number of workers per firm to the counter (as the vintage is fully utilized)
						v[15] += v[22] * v[19]; //adds the (expected) emissions to produce the consumption goods with this vintage
					} 
					else // if the number of goods that still need to be produced is smaller than the max production of the vintage
					{
						v[13] = v[23] / v[21]; // total number of workers that the vintage will employ to produce the remaining goods
							
						v[11] += v[23]; // total number of goods that the firm will produce
						v[12] += v[13]; // total number of workers that the firm will employ
						v[15] += v[23] * v[19]; //total CO2 that the firm will emit from production
					}
				} // close if
			}

				WRITE("_emissions_c_expected", v[15]);

				v[30] = VS(PARENT, "manager_per_worker_c"); 
				v[31] = round( v[12] ) * v[30];
				v[32] = round( v[31] );

				WRITE("_managers_c_desired", v[32]);

	RESULT( round (v[12]) )

	EQUATION("_workers_c_demand")
		/*UNIT: number of workers
		
		Actual demand for workers after budget is distributed*/

		v[0] = V("_cost_production_budget_c") / V("_cost_unit_production_c_expected"); //expected production after budget distributed

		v[11] = 0; // sets counter of produced goods to zero
		v[12] = 0; // sets counter of number of workers needed to zero

		SORT("vintage", "__productivity_vintage", "DOWN"); // sorts the machines down (from more to less productive)
	
		CYCLE(cur, "vintage") // cycle for the sorted machines
		{
	
			v[20] = VS(cur, "__worker_vintage_max"); // maximum number of workers employed at the vintage (to produce the maximum quantity) - considering all its machines
				
			v[21] = VS(cur,"__productivity_vintage"); // productivity of the the vintage
				
			v[22] = VS(cur, "__capital_vintage"); // maximum number of goods that can be produced by all the vintage's machines at full capacity utilization

			v[23] = v[0] - v[11]; // number of goods that still need to be produced to reach the desired production level

			// current production
				if(v[23] > 0 && v[21] > 0) // if the number of goods produced so far is smaller than the number of goods the firm wants to produce -> the vintage will be used
				{
					if(v[23] >= v[22]) // if the number of goods that still need to be produced is larger than the max number of goods the vintage can produce, it will be entirely utilized
					{
						v[11] += v[22]; // adds the number of goods produced by the firm to the counter of goods
						v[12] += v[20]; // adds the max number of workers per firm to the counter (as the vintage is fully utilized)
					} 
					else // if the number of goods that still need to be produced is smaller than the max production of the vintage
					{
						v[13] = v[23] / v[21]; // total number of workers that the vintage will employ to produce the remaining goods
							
						v[11] += v[23]; // total number of goods that the firm will produce
						v[12] += v[13]; // total number of workers that the firm will employ
					}
				} // close if
			}

				v[30] = VS(PARENT, "manager_per_worker_c"); 
				v[31] = round( v[12] ) * v[30];
				v[32] = round( v[31] );

				WRITE("_managers_c_demand", v[32]);

	RESULT( round(v[12]) )


	EQUATION("_workers_c_employed")
		/* UNIT: number of workers
		
		Effective number of workers available to production of c goods*/

		v[2] = V("_workers_c_demand") * VS(hh_sector2, "labor_restriction"); //potential shortage is proportional to firm initial demand
		
	RESULT( round(v[2]) )

	EQUATION("_managers_c_employed")
		/* UNIT: number of workers
		
		Effective number of workers available to production of c goods*/

		v[2] = V("_managers_c_demand") * VS(hh_sector2, "labor_restriction"); //potential shortage is proportional to firm initial demand
		
	RESULT( round(v[2]) )


// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// PART 3: PRODUCTION --------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================

	EQUATION("_production_c") 
		/* UNIT: number of goods

		Number of goods effectively produced by the firm, after direct workers were hired */


		v[0] = V("_workers_c_employed"); // number of workers employed at the firm

		if(v[0] == 0)
		{
			END_EQUATION( 0 );
		}

		v[21] = 0; // sets counter of number of goods produced to zero
		v[22] = 0; // sets counter of emissions to zero

		SUM("__productivity_vintage"); // guarantee that all vintages are counted

		SORT("vintage", "__productivity_vintage", "DOWN"); // sorts the vintages down (from more to less productive)

		CYCLE(cur, "vintage") // cycle for the sorted vintages
		{
			v[1] = VS(cur, "__worker_vintage_max"); // maximum number of workers at the vintage
			v[2] = VS(cur , "__green_productivity_vintage"); //emissions coefficient of the vintage's machine
			v[10] = VS(cur,"__productivity_vintage") ; // productivity of the vintage's machines

			v[11] = min( v[0] , v[1] ); // number of workers employed at the vintage
			
			v[0] -= v[11]; // updates the number of workers employed and still available to be allocated at a vintage
			v[21] += ( v[10] * v[11] ); // updates the number of goods produced
			v[22] += v[10] * v[11] * v[2];
		}

		WRITE("_emissions_c", v[22]);
	
	RESULT( v[21] )	


	EQUATION("_capacity_ut_c")
		/* UNIT: rate.
		
		Capacity utilization rate in the current period */
	
		v[0] = V("_production_c"); 
		v[1] = V("_capital_stock_c");
		
		v[2] = (v[1] <= 0) ? 0 : (v[0] / v[1]);
		
	RESULT( v[2] )
			

// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// PART 4: INVESTMENT  --------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================

	EQUATION( "_investment_desired_expansion_c" )
		/* UNIT: number of K goods
		
		Desired expansion investment of firm in consumption-good sector*/

		v[1] = V( "_capacity_ut_desired_c" );												// desired capacity utlization
		v[2] = SUM("__capital_vintage");							// available capital stock

		v[5] = ( VS( PARENT, "gamma_a" ) + VS( PARENT, "gamma_u") * ( v[1] - VS(PARENT, "capacity_ut_normal" ) ) ) * v[2]; // flexible accelerator mechanism with autonomous investment

		v[6] = max( v[5], 0);

	RESULT( v[5] )


	EQUATION( "_investment_desired_substitution_c" )
		/* UNIT: number of K goods
		
		Desired substitution investment of firm in consumption-good sector*/

		v[1] = VL("_deleted_capital", 1);          //deleted capital
												
		v[2] = VL("_capital_stock_c", 1) * VS(PARENT, "delta"); //depreciated capitaL
		v[0] = v[1] + v[2]; 

	RESULT( v[0] )
	

	EQUATION("_investment_desired_c")
		/* UNIT: number of capital goods.
		 
		Number of capital goods to be desired in the current period (still to be adjusted by financial restrictions) */

		v[0] = V("_investment_desired_substitution_c") + V("_investment_desired_expansion_c");
 	RESULT( v[0] )

 
	EQUATION("_investment_demand_c") 
		/* UNIT: number of capital goods

		Number of capital goods effectively demanded, after financial restrictions are applied  */

		VS(k_sector2, "clients_new_k");
		V( "_supplier" );

		cur = PARENTS( SHOOKS( SHOOK ) ) ;// hook current supplier

		V("_loan_new_c"); // new amount of loans has to be define before demand is set

		v[0] = V("_cost_investment_budget"); // financial resources available for investment

		v[1] = V("_investment_desired_c"); // desired investment demand
		
		v[2] = VS(cur, "_price_k"); // price of capital goods

		v[3] =  v[0] / v[2] ; // number of capital goods that the firm can purchase

		// precision adjustment for number of machines
			
			if(v[3] < v[1]) 
			{
				v[10] = (v[1] * v[2]) - v[0]; // difference in budget
				
				if(V("_deposit_left_loan_c") >= v[10])
				{
					INCR("_deposit_left_loan_c", -v[10]);
					v[3] = v[1];
				}
				else
				{
					if( V("_loan_constrained_c_dummy") == 0 ) // if the firm is not financially constrained and it was worth financing investment through loan -> precision adjustment
					{
						INCR("_loan_new_c", v[10]); // adjust amount of loans by difference required to reach desired investment
						v[3] = v[1];
					}

				}
			}

		v[4] = min( v[3], v[1] ); // check just to be sure
		v[5] = max( v[4], 0 ); // investment demand cannot be negative

		send_order( THIS, v[5] ); //send order from THIS firm to its supplier in the size of v[5]

	RESULT( v[5] )	


	EQUATION( "_supplier" )
		/* UNIT: K Firm id

		Selected machine supplier by firm in consumption-good sector. Also set firm 'hook' pointers to supplier firm object*/

		VS(k_sector2, "clients_new_k" );					// ensure brochures are sent
		VS(k_sector2, "productivity_k_avg" );				// ensure innovation is done

		v[1] = VS(government2, "CO2_tax");												
		v[2] = VL("_wage_c", 1);
		v[3] = v[2] / VS(k_sector2, "productivity_c_new_min") + v[1] * VS(k_sector2, "green_productivity_c_new_max"); //max supplier new cost
		v[4] = VS(k_sector2, "price_k_max") + v[3];									// max supplier price+cost
		i = 0;
		cur2 = cur3 = NULL;
		CYCLE_SAFE( cur, "broch" )							// use brochures to find supplier
		{
				cur1 = PARENTS( SHOOKS( cur ) );			// pointer to supplier object

				// compare capital price + cost
				v[5] = VS( cur1, "_price_k" );
				v[6] =  v[2] / VS(cur1, "_productivity_c_new") + v[1] * VS(cur1, "_green_productivity_c_new");
				v[7] = v[5] + v[6];

				if ( v[7] < v[4] )							// best so far?
				{
					v[4] = v[5];							// save current best supplier
					i = VS( cur1, "_id_firm_k" );					// supplier ID
					cur2 = SHOOKS( cur );					// own entry on supplier list
					cur3 = cur;								// best supplier brochure
				}	
			
		}

		// if supplier is found, simply update it, if not, draw a random one
		if ( cur2 != NULL && cur3 != NULL )
		{
			WRITES( cur2, "__tSel", T );				// update selection time
			WRITE_SHOOK( cur3 );					// pointer to current brochure
		}
		else											// no brochure received
		{
			cur1 = set_supplier( THIS );				// draw new supplier
			i = VS( cur1, "_id_firm_k" );	
		}	

		v[20] = COUNT("broch");

		WRITE("_brochures", v[20])

	RESULT( i )


	EQUATION( "_investment_canceled_c" )
		/* UNIT: number of capital goods
	
		Canceled investment of firm in consumption-good sector*/

		V( "_supplier" );								// ensure new supplier is set

		cur = SHOOK;							// pointer to current supplier
		VS( PARENTS( SHOOKS( cur ) ), "_production_k" );			// make sure supplier produced

		v[1] = VS( SHOOKS( cur ), "__nCan" );			// canceled machine number

	RESULT( v[1] )


	EQUATION( "_investment_realized_c" )
		/* UNIT: number of capital goods
	
		Realized investment of firm in consumption-good sector*/

		v[1] = V("_investment_demand_c") - V("_investment_canceled_c");

		v[2] = v[1] - V("_investment_desired_substitution_c");

		WRITE("_investment_realized_net_c", v[2]);

	RESULT( v[1] )

	EQUATION_DUMMY("_investment_realized_net_c", "_investment_realized_c")
	

	EQUATION("_creation_new_vintage") 
		/* UNIT: number of capital goods

		Returns the realized investment. It also creates the vintage with the new capital */
		V("_delete_vintage");
		v[0] = V("_investment_realized_c"); // number of machines the firm will receive
		

		object *suppl;

		suppl = PARENTS( SHOOKS( SHOOK ) ) ;// current supplier

		v[1] = VS(suppl, "_productivity_c_new"); // new productivity of the machines
		v[3] = VS(suppl, "_green_productivity_c_new"); //new emissions coefficient

		WRITE("_productivity_c", v[1]);
		
		if( v[0] > 0 ) // if there are new machines to be added 
		{
			cur = ADDOBJ( "vintage"); // copies the object vintage
			WRITES(cur, "__capital_vintage", v[0]); // update total capital in the vintage
			WRITELS(cur, "__capital_vintage", v[0], 1); // update total capital in the vintage
			WRITES(cur, "__init_capital_vintage", v[0]); // update the new capital vintage
			WRITES(cur, "__productivity_vintage", v[1]); // updates the productivity of the new capital good
			WRITES(cur, "__vintage_date", T); // period in which the capital was acquired
			WRITES(cur, "__green_productivity_vintage", v[3]); //updates the emissions coefficient of the new capital good

		}
	
	RESULT( v[0] )	

	EQUATION("_delete_vintage")
		/*UNIT: number of vintages
		
		Delete too depreciated vintages*/

		v[0] = 0; //counter of deleted firms
		v[1] = 0; //counter of lost capital
		 
		if( VL("_vintages", 1) > 2){

			CYCLE_SAFE(cur, "vintage"){
				if( VS(cur, "__capital_vintage") < VS(cur, "__init_capital_vintage") * VS(GRANDPARENT, "min_capital_delete")){ //if capital is below a treshold of its original value, scrap vintage
				
				v[1] += VS(cur, "__capital_vintage");

				DELETE( cur );							// remove vintage entry

				v[0] ++;  								//count how many scraped vintages

				

				}
			}

		}
		WRITE("_vintages", COUNT("vintage") );

		WRITE( "_deleted_capital", v[1] );
		
		

	RESULT( v[0] ) //expected to be only 1 vintage per period


	EQUATION( "_capital_stock_c" )
		/* UNIT: number of K goods
		
		Stock of capital in all vintages for each K Firm (it is also the full capacity production) */

		v[0] = 0; //capital counter

		V("_creation_new_vintage");
		V("_delete_vintage");

		CYCLE(cur, "vintage") // cycle for the sorted vintages
		{
			v[1] = VS(cur, "__capital_vintage"); // maximum number of workers at the vintage
			
			v[0] += v[1]; // updates the stock of capital			
		}

	RESULT( v[0] )	

	EQUATION("_productivity_c_growth")
		/* UNIT: rate

		Growth rate of productivity of new machines */

		v[0] = V("_productivity_c");
		v[1] = VL("_productivity_c", 1);

		v[2] = (v[1] == 0)? 0: ( v[0] / v[1] ) - 1;

	RESULT(v[2])

	EQUATION("_investment_rate_c")
	/* UNIT: rate

		Investment rate (I/K) */

		v[0] = V("_investment_realized_net_c");
		v[1] = VL("_capital_stock_c", 1);

		v[2] = (v[1] == 0)? 0: ( v[0] / v[1] );

	RESULT(v[2])

	
	
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// PART 5: FINANCIAL CONSTRAINT --------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================

	EQUATION("_cost_loan_repayment_due_c")  
		/* UNIT: monetary units

		Amount of loans that needs to be repaid by the firm to the bank in the beginning of the current period*/

		v[0] = VL("_loan_c", 1); // outstanding debt from the previous period
		v[1] = VLS(financial2, "interest_rate_loan", 1); // interest rate from the previous period

		v[2] = v[0] * ( 1 + v[1] );

		WRITE("_cost_loan_interest_due", v[0] * v[1]);

	RESULT( v[2] )

	EQUATION("_loan_c_demand") 
		/* UNIT: monetary units

		Total of loans demand from firm. */

		v[0] = V("_cost_loan_repayment_due_c"); // cost with repaying previous debt
		v[1] = V("_cost_production_expected_c"); // expected production cost
		v[2] = V("_cost_investment_expected_c"); // expected investment cost

		v[3] = VL("_deposits_c", 1); // deposits available for covering costs in the period

		// demand for loans 

			v[20] = v[0] + v[1] + v[2] - v[3]; // total amount that needs to be financed
		
			v[21] = max(v[20], 0); // demand for loans

	RESULT( v[21] )


	EQUATION("_loan_new_c") 
		/* UNIT: monetary units

		Total of loans to the firm. Also updates budget for each type of activity */

		v[0] = V("_cost_loan_repayment_due_c"); // cost with repaying previous debt
		v[1] = V("_cost_production_expected_c"); // expected production cost
		v[2] = V("_cost_investment_expected_c"); // expected investment cost

		v[3] = VL("_deposits_c", 1); // deposits available for covering costs in the period

		// demand for loans 

			v[21] = V("_loan_c_demand");
			v[22] = VS(PARENT, "loan_c_demand");

			v[23] = VS(financial2, "bank_debt_supply_max");

			v[24] = v[0] - v[3]; // amount of funds needed to repay debt

		// amount of loans obtained by firms
			//v[30] = V("_revenue_average_c");
			//v[31] = VS(financial2, "phi"); // max ratio between interest payments and revenue 

			v[31] =  v[22] > v[23] ? v[23] : v[22]; 				//  restriction?

			v[32] = ( v[21] > 0 ) ? v[21] *  (v[31] / v[22]) : 0 ;  //  proportional distribution of credit demand

			v[33] = max( v[32], v[24]); 							//  amount of credit is at least equal to the amount needed to cover the amount needed for debt repayment	

		// financial constrain dummy?

			v[43] = (v[33] < v[21]) ? 1 : 0;

			WRITE("_loan_constrained_c_dummy", v[43]);

		// budget for each activity (priority order: debt repayment, production, investment): 

			v[50] = v[3] + v[33]; // total amount of resources available

			v[70] = 0; // set accumulator of difference in loan to zero

			// debt repayment

				v[51] = min(v[0], v[50]);

				if(v[43] == 0 && v[51] < v[0]) // if firm is not financially constrained but credit was lower than demanded for this category 
				{
					v[70] += (v[0] - v[51]); // difference in loan is accumulated

					v[51] = v[0]; // precision adjustment
				}

				WRITE("_cost_loan_repayment_budget_c", v[51]);

				v[52] = v[50] - v[51]; // resources left

			// production

				v[53] = min(v[1], v[52]);

				if(v[43] == 0 && v[53] < v[1]) // if firm is not financially constrained, it is worth to finance production with credit and budget was lower than expected production costs
				{
					v[70] += (v[1] - v[53]); // difference in loan is accumulated

					v[53] = v[1]; // precision adjustment
				}

				WRITE("_cost_production_budget_c", v[53]);

				v[54] = v[52] - v[53]; // resources left

			// investment

				v[55] = min(v[2], v[54]);

				if(v[43] == 0 && v[55] < v[2]) // if firm is not financially constrained, it is worth to finance investment with credit and budget was lower than expected investment costs
				{
					v[70] += (v[2] - v[55]); // difference in loan
 
					v[55] = v[2]; // precision adjustment
				}

				WRITE("_cost_investment_budget", v[55]);
				
				v[56] = v[54] - v[55]; // resources left

				WRITE("_deposit_left_loan_c", v[56] + v[70]); // amount of resources left after receiving new loan (expected to be left after costs are paid)

		// precision adjustment

			v[44] = v[33] +  v[70];

	RESULT( v[44] )	


	EQUATION("_loan_c") 
		/* UNIT: monetary units

		Total amount of outstanding loans in the period */

		V("_investment_demand_c"); // calculated only after investment demand (precision adjustments in loan_new_c can occur before that)

		v[2] = V("_cost_loan_repayment_due_c"); // amount that has to be repaid

		v[3] = V("_cost_loan_repayment_budget_c"); // amount available for loan repayment

		v[4] = V("_loan_new_c"); // new loan obtained by the firm

		v[10] = v[2] - v[3] + v[4]; // new outstanding loan

		// checks

			v[5] = VL("_deposits_c", 1);
			
			v[11] = v[4] + v[5] - v[3]; // value to check whether the firm can repay the loan

			if( v[11] < 0 && abs(v[11] / v[3]) > VS(GRANDPARENT, "error_margin_accepted")) {PLOG("\n -> WARNING: new loan plus deposits insufficient to cover previous period debt - difference equal to %.2lf %.", v[11]/v[3]);}

	RESULT(v[10])	

	
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// PART 6: PRICE, COSTS AND SALES --------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================

	EQUATION("_price_c") 
		/* UNIT: monetary units

		Price charged by the firm on its consumption goods. It considers the mark-up defined in the C goods market and in relation to workers. Equation also allows price reviews to be separated from price adjustments, which will depend on a tolerance band around the reference mark-up level - to deactivate this mechanism just set mark_up_deviation_c = 0. */
		
		v[7] = V("_cost_unit_production_c_expected"); // costs considered in pricing equation
		
		v[8] = V("_mark_up_desired_c"); // reference mark-up for firms
		v[9] = VL("_price_c", 1);
		
		v[10] = V("_production_desired_c");		

		if(  v[10] > 0 ) // production at desired capacity utilization rate (unit costs are > 0)
			{
				v[20] = v[7] * ( 1 + v[8] );
			}
			else // if the firm has no production capacity in the period
			{
				END_EQUATION( v[9] ); // it keeps the previous price level -> will guide investment decisions and is the price at which inventories will be sold
			}

		// minimum price level to guarantee that taxes are covered and mark up rate is equal to minimum 
			
			v[21] =  VS(government2, "tax_rate_sales");

			v[22] = ( 1 + v[21] / ( 1 - v[21] ) ) * v[7] * (1 + VS(PARENT, "mark_up_min_c") ); 

		// price level

			v[30] = max( v[20], v[22] );

			WRITE("_green_price_subsidie_c", 0);

	RESULT(v[30])

	EQUATION_DUMMY("_green_price_subsidie_c", "_price_c")

	EQUATION("_mark_up_c")
		/* UNIT: ratio

		Effective mark-up charged by the firm */
		v[0] = V("_price_c");
		v[1] = V("_cost_unit_production_c");

		v[2] = (v[1] > 0) ? ((v[0] / v[1]) -1) : VL("_mark_up_c", 1); // if costs were zero, price was also zero and mark-up was irrelevant (so just copy previous value)

	RESULT( v[2] )	

	EQUATION("_market_share_c_before_rescale") 
		/* UNIT: ratio

		Firm's market share (of nominal consumption demand) - this is a preliminary value (yet to be adjusted by ms rescale) */

 		v[0] = VL("_market_share_effective_c", 1); // previous period market share 
		v[1] = V("_competitiveness_c"); // firm's competitiveness

		v[2] = VS(PARENT, "competitiveness_average_c"); // average competitiveness in the sector 
		v[3] = VS(PARENT, "sensitivity_market_share_competitiveness"); // sensitivity of firm's market share to competitiveness

		v[4] = v[3] * ( v[1] / v[2] - 1 ) ; // adjustment factor
		
		v[5] = v[0] * ( 1 + v[4] );

		v[6] = min(v[5], 0.1) //tendency to concentrate the market, we limite this

	RESULT( v[5] )

	EQUATION("_market_share_effective_c" ) 
		/* UNIT: ratio

		Effective market share in the period (only considering domestic firms). This is the firms' realize proportion of real sales in the period (can differ from market share due to lack of supply) */

		v[0] = V("_sales_c_real");
		v[1] = VS(PARENT, "sales_c_real");

		v[2] = ( v[1] > 0 ) ? v[0] / v[1] : 0;

	RESULT( v[2] )


	EQUATION("_demand_c_real")
		/* UNIT: number of goods

		Demand for firms' products */

		v[1] = VS(PARENT, "market_share_c_rescale"); 

		v[0] = V("_market_share_c"); // firm's market share

		v[5] = VS(GRANDPARENT, "demand_c_real")  * v[0] / v[1]; // demand for the firm's goods (in real terms)

		v[6] = VL("_green_c",1) == 1? v[5] + VS(government2, "green_public_procurement_c") : v[5];
	
	RESULT( v[6] )


	EQUATION("_unfilled_demand_c")
		/* UNIT: number of goods

		Quantity of goods demanded from firm that it could not sell */

		v[1] = V("_demand_c_real");

		v[2] = V("_sales_c_real");

		v[3] = v[1] - v[2];

	RESULT( v[3] )


	EQUATION("_sales_c_real") 
		/* UNIT: number of goods

		Quantity sold by the firm. When it is an open economy, this equation also sets the share of exported goods by the firm. */

		v[5] = V("_demand_c_real");
		v[6] = V("_production_c"); 
		v[7] = VL("_inventories_c", 1);
		v[8] = v[6] + v[7]; // goods available in the period that can be sold by the firm

		v[9] = min(v[8], v[5]); // number of goods sold

	RESULT( v[9] )

	EQUATION("_inventories_c") 
		/* UNIT: number of consumption goods

		Inventories are given by the difference between current production and sales plus past inventories */

		v[0] = V("_production_c");
		v[1] = V("_sales_c_real");
		v[2] = VL("_inventories_c", 1);

		v[3] = v[0] + v[2] - v[1];

		v[4] = max( v[3], 0 ); // just to be sure (precision adjustment, if any)

	RESULT( v[4] )

	EQUATION("_inventories_share_c")
		/* UNIT: ratio

		Inventories as a share of sales */

		v[0] = V("_inventories_c");
		v[1] = V("_sales_c_real");

		v[2] = (v[1] > 0) ? ( v[0] / v[1] ) : 0;	

	RESULT( v[2] )

	EQUATION("_demand_c_growth")
		/* UNIT: rate

		Growth rate of the demand for the firm's production */ 

		v[0] = V("_demand_c_real");
		v[1] = VL("_demand_c_real", 1);

		v[2] =  ( v[1] > 0 ) ? ( v[0] / v[1] ) - 1 : 0;

	RESULT( v[2] )	

	
	EQUATION("_competitiveness_c")
		/* UNIT: irrelevant

		Firm's competitiveness in the goods market: depends on price level and unfilled demand. Values are normalized to [0.1, 0.9]: zero competitiveness is avoided to prevent direct exit of worst firms in replicator dynamics (as in KS model) */

		VS(PARENT, "price_c_average_before_ms"); 
		VS(PARENT, "unfilled_demand_c_average"); 

		v[1] = VS(PARENT, "price_c_min");
		v[2] = VS(PARENT, "price_c_max");		
		v[3] = V("_price_c"); // firm's price

		v[4] = (v[2] > v[1]) ? 0.1 + 0.8 * ( ( v[3] - v[1] ) / ( v[2] - v[1] ) ) : 0.5; // normalized price

		v[5] = VS(PARENT, "sensitivity_competitiveness_price"); // sensitivity of firm's competitiveness to its  price		

		v[11] = VS(PARENT, "unfilled_demand_c_min"); // unfilled demand min (already value from the previous period)
		v[12] = VS(PARENT, "unfilled_demand_c_max"); // unfilled demand max (already value from the previous period)
		v[13] = VL("_unfilled_demand_c", 1); // level of unfilled demand

		v[14] = (v[12] > v[11]) ? 0.1 + 0.8 * ( ( v[13] - v[11] ) / ( v[12] - v[11] ) ) : 0.5; // normalized unfilled demand

 		v[15] = VS(PARENT, "sensitivity_competitiveness_unfilled_demand"); // sensitivity of firm's competitiveness to unfilled demand

 		v[20] = ( v[5] * (1 - v[4]) + v[15] * (1 - v[14]) ) / ( v[5] + v[15] );

	RESULT( v[20] )


// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// PART 7: ACCOUNTING EQUATIONS ----------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================


	EQUATION("_sales_c_nominal") 
		/* UNIT: monetary units

		Nominal sales of C goods by the firm (revenue) - this considers both exports and imports */

		v[1] = V("_price_c");
		v[0] = V("_sales_c_real") ;

		v[2] = v[1] * v[0] ;

 	RESULT( v[2] )


	EQUATION("_cost_investment_c") 
		/* UNIT: monetary units

		Effective investment cost */

		cur = PARENTS( SHOOKS( SHOOK ) ) ;// hook current supplier

		v[0] = V("_investment_realized_c"); // number of capital goods the firm acquired
		v[1] = VS(cur, "_price_k"); // price of the capital good

		v[2] = v[0] * v[1]; // cost with investment

	RESULT( v[2] )		


	EQUATION("_cost_production_c") 
		/* UNIT: monetary units

		Total production costs with wages */ 

		v[0] = V("_emissions_c") * VS(government2, "CO2_tax"); //cost with emissions
		
		v[1] = V("_cost_workers_c"); //cost with workers

		v[2] = V("_cost_managers_c"); //cost with managers

		v[3] = v[1] + v[0] + v[2]; 

	RESULT( v[3] )


	EQUATION("_cost_workers_c")
		/* UNIT: monetary units

		Cost with employed workers in the consumption goods sector */
	
		v[0] = V("_workers_c_employed");
		v[10] = V("_wage_c");

	RESULT( v[0] * v[10] )	


	EQUATION("_cost_managers_c")
		/* UNIT: monetary units

		Cost with employed managers in the consumption goods sector */
	
		v[0] = V("_managers_c_employed");
		v[10] = V("_managers_wage_c");

	RESULT( v[0] * v[10] )
	

	EQUATION("_cost_unit_production_c") 
		/* UNIT: monetary units

		Effective unit costs before hiring and effective demand.  */

		v[1] = V("_cost_production_c"); // demand for direct workers has to be updated before

		v[2] = V("_production_c"); // production at desired capacity utilization rate

		v[11] = ( v[2] == 0 ) ? 0 : v[1] / v[2] ;

	RESULT( v[11] )		


	EQUATION("_taxes_sales_paid_c")
		/* UNIT: monetary units
		
		Taxes paid by firm sales by the capital goods firm */

		v[0] = V("_sales_c_nominal") * VS(government2, "tax_rate_sales") ;

	RESULT( v[0] )	


	EQUATION("_profits_gross_c") 
		/* UNIT: monetary units

		Gross profits from accounting point of view */ 
	
		v[0] = V("_sales_c_nominal"); 
		v[1] = V("_cost_production_c"); 
		v[2] = V("_taxes_sales_paid_c"); 

		v[4] = V("_inventories_c") - VL("_inventories_c", 1); // change in inventories
		v[5] = V("_cost_unit_production_c"); // cost at which inventories are evaluated
		v[6] = v[4] * v[5];
	
		v[10] = v[0] - v[1] - v[2] + v[6];	

	RESULT( v[10] )


 	EQUATION("_profits_net_c")
 		/* UNIT: monetary units 

 		Profits discounting interest payment and adding interest received from bank, change in inventories value (no capital amortization) - this is the amount of profits considered in dividends distribution */

 		v[0] = V("_profits_gross_c");
 		
 		v[2] = V("_cost_loan_interest_due"); // cost that the firm had with interest payments in this period

 		v[3] = VL("_inventories_c", 1);
 		v[4] = V("_cost_unit_production_c") - VL("_cost_unit_production_c", 1); // capital gain per unit of inventory
 		v[5] = v[3] * v[4]; // capital gain on inventories

 		v[6] = v[0] - v[2] + v[5];

 	RESULT( v[6] )

	EQUATION("_profits_distributed_c") 
 		/* UNIT: monetary units

 		Amount of profits to be distributed to capitalists who own the firm: it depends on previous profits. It can be divided in a certain number of periods for smoothing. */	

 		int _i; // initializes counter
 		double _j = VS(PARENT, "profits_distributed_periods"); // periods that are considered in profits

 		for( v[0] = 0, _i = 0; _i < _j ; _i++ )
 		{
 			v[0] += VL("_profits_net_c", _i);
 		}

 		v[1] = v[0] / _j; // average profits

 		v[2] = V("_loan_new_c") + VL("_deposits_c", 1) + V("_sales_c_nominal"); // source of funds

 		v[3] = V("_cost_loan_repayment_budget_c") + V("_cost_production_c") + V("_cost_investment_c") + V("_taxes_sales_paid_c"); // used resources in the period

		v[4] = v[2] - v[3] - V("_loan_new_c"); // resources available in the period for redistribution (firm keeps the amount of loans from current period to be repaid in the next period, if possible)

		v[5] = VS(PARENT, "dividends_target_c");

		v[6] = v[1] * v[5]; // amount of profits to be distributed

 		v[10] = max( min(v[6], v[4]), 0); // profits that are distributed

		v[11] = v[10] / VS(GRANDPARENT, "capitalists_per_firm");

		WRITE("_dividends_c", v[11]);

	RESULT( v[10] )

	
	EQUATION("_deposits_c") 
		/* UNIT: monetary units

		Deposits kept at the bank at the end of the period */

		v[0] = VL("_deposits_c", 1); // deposits in the beginning of the period (already discounted wealth tax)
		v[1] = V("_loan_new_c"); // new loan
		v[2] = V("_sales_c_nominal"); // firm's revenue
		v[10] = v[0] + v[1] + v[2]; // total resources available

		v[3] = V("_cost_loan_repayment_budget_c"); // cost with previous loan repayment
		v[4] = V("_cost_investment_c"); // effective cost with investment
		v[5] = V("_cost_production_c"); // effective cost with production
		v[6] = V("_taxes_sales_paid_c"); // taxes paid on sales
		v[7] = V("_profits_distributed_c"); // distributed profits
		v[11] = v[3] + v[4] + v[5] + v[6] + v[7]; // total costs incurred

		v[20] = v[10] - v[11];


		if( v[20] < 0) {
			//INTERACT("_deposits_c", v[20]);
			
			//LOG("\n -> WARNING: negative deposits in time %.0lf for firm %.0lf", T , V("_id_firm_c") );

			//INCR("_loan_c", -v[20]);

			}
	
	RESULT(v[20])

	EQUATION("_revenue_average_c") 
		/* UNIT: monetary units

		Average nominal revenue of the firm in previous periods (in current period price level).  */

		v[0] = VL("_sales_c_real",1);
		v[1] = VL("_sales_c_real",2);
		v[2] = VL("_sales_c_real",3);
		v[3] = VL("_sales_c_real",4);

		v[4] = V("_price_c") * (v[0]+v[1]+v[2]+v[3])/4; // nominal average

	RESULT( v[4] )


// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// PART 8: ENVORINMENTAL EQUATIONS ----------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================


	EQUATION("_green_productivity_vintage_average")
		/*UNIT: tons of CO2 per C good produced
		
		Average green productivity in the stock of capital from C firm*/

		v[0] = AVE("__green_productivity_vintage")

	RESULT( v[0] )



// -----------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// DUMMY VARIABLES -------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// -----------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================


	EQUATION_DUMMY("_cost_loan_interest_due", "_cost_loan_repayment_due_c")

	EQUATION_DUMMY("_productivity_c", "_creation_new_vintage")

	EQUATION_DUMMY("_wage_c_growth", "_wage_c")

	EQUATION_DUMMY("_loan_constrained_c_dummy", "_loan_new_c")

	EQUATION_DUMMY("_cost_loan_repayment_budget_c", "_loan_new_c")

	EQUATION_DUMMY("_cost_production_budget_c", "_loan_new_c")

	EQUATION_DUMMY("_cost_investment_budget", "_loan_new_c")

	EQUATION_DUMMY("_deposit_left_loan_c", "_loan_new_c")

	EQUATION_DUMMY("_emissions_c_expected", "_workers_c_desired")

	EQUATION_DUMMY("_managers_c_demand", "_workers_c_demand")

	EQUATION_DUMMY("_managers_c_desired", "_workers_c_desired")

	EQUATION_DUMMY("_emissions_c", "_production_c")

	EQUATION_DUMMY("_capacity_ut_desired_c", "_production_desired_c")

	EQUATION_DUMMY("_dividends_c", "_profits_distributed_c")

	EQUATION_DUMMY("_brochures", "_supplier")

	EQUATION_DUMMY("_vintages", "_delete_vintage")

	EQUATION_DUMMY("_green_c", "green_c")

	EQUATION_DUMMY("_deleted_capital", "_delete_vintage")

	EQUATION_DUMMY("_market_share_c", "market_share_c_rescale")

	

	
