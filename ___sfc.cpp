/******************************************************************************

	CHECK EQUATIONS
	----------------------

	Written by Lucca Rodrigues, Made-USP

	Equations that check for stock flow consistency and other aggregate indicators in the Made-Green model
	are coded below.

 ******************************************************************************/


// -----------------------------------------------------------------------------------------------------------------------
// 0. AGGREGATE CHECK ----------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------
	
	EQUATION("checks")
		/* UNIT: percentage

		Sums all checks (which are in percentages of the variables tested): if it is zero, all is fine */

		VS(PARENT, "order_equations"); // Simulations in current period must have finished.

		// 1. Transaction flow matrix checks use 

			v[10] = V("check_flows_hh");
			v[10] += V("check_flows_firm_c");
			v[10] += V("check_flows_bank");
			v[10] += V("check_flows_firm_k");
			v[10] += V("check_flows_government");

			v[10] += V("check_flows_consumption");
			v[10] += V("check_flows_wages");
			v[10] += V("check_flows_profits");
			v[10] += V("check_flows_deposits");
			v[10] += V("check_flows_loan");

		// 2. Balance sheet matrix

			v[10] += V("check_stocks_loan");
			v[10] += V("check_stocks_deposits");

		// 3. Other checks 

			v[10] += V("check_sales");
			v[10] += V("check_macro_aggregates");
			v[10] += V("check_macro_aggregates_real");

		if(t==1) LOG("\n Model's consistency checked");

	RESULT( v[10] )

// -----------------------------------------------------------------------------------------------------------------------
// 1. TRANSACTION FLOW MATRIX CHECKS -----------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

	// 1.a. VERTICAL CHECKS 

		EQUATION("check_flows_hh")
			/* UNIT: percentage points

			Checks the consistency in households' flows 
			
			organization:
				v[20] -- v[29] > source of funds
				v[31] -- v[39] > use of funds
			*/

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

			v[21] = VS(hh_sector0, "income_total_hh"); // income (wages, dividends, unemployment benefits)
			v[20] = v[21] ; // sources of funds

			v[31] = VS(hh_sector0, "consumption_realized_hh"); // realized consumption (from domestic and foreign sectors)
			v[32] = VS(hh_sector0, "deposits_hh") - VLS(hh_sector0, "deposits_hh",1); // change in amount of deposits held
			v[33] = VS(hh_sector0, "total_taxes_hh");
			v[30] = v[31] + v[32] + v[33] ; // use of funds

			v[10] = v[20] == 0 ? 1: ( v[20] - v[30] ) / v[20]; // should be equal to zero
			
			v[11] = abs( v[10] );

			if(v[11]>=V("check_threshold"))	LOG("\n -> WARNING: error in check HH flows equal to  %.2lf in time %.0lf", v[11], T );

		RESULT( v[11] )

		EQUATION("check_flows_firm_c")
			/* UNIT: percentage points

			Checks the consistency of C firms' flows. Adjusts values by considering also firms that exited the market.	*/

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

			v[40] = VS(financial0, "loans_unpaid_exited_firms"); // loans unpaid by firms that exited
			v[41] = VS(financial0, "loans_paid_exited_firms"); // loans paid by firms that exited

			// sources of funds
			v[20] = VS(c_sector0, "sales_c_nominal"); // sales to domestic and foreign sectors
			v[20] += ( VS(c_sector0, "loan_c")  + v[40] + v[41] ) - VLS(c_sector0, "loan_c", 1);

			// use of funds
			v[30] = VS(c_sector0, "cost_workers_c");
			v[30] += VS(c_sector0, "cost_managers_c");
			v[30] += VS(c_sector0, "cost_investment_nominal_c");
			v[30] += VS(c_sector0, "interest_paid_c");
			v[30] += VS(c_sector0, "profits_distributed_c"); 
			v[30] += VS(government0, "tax_rate_sales") * VS(c_sector0, "sales_c_nominal");
			v[30] += ( VS(c_sector0, "deposits_c") + v[41] ) - VLS(c_sector0, "deposits_c", 1);

			v[10] = ( v[20] - v[30] ) / v[20]; // should be equal to zero
			
			v[11] = abs( v[10] );

			if(v[11]>=V("check_threshold")) 
			{
				LOG("\n -> WARNING: error in check C firms flows equal to  %.2lf in time %.0lf",v[11],T);
			}	


	 	RESULT( v[11] )

	 	EQUATION("check_flows_firm_k")
			/* UNIT: percentage points

			Checks the consistency of K firm's flows 

			organization:
				v[20] - v[29] > source of funds
				v[31] - v[39] > use of funds
			*/

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

	 		v[20] = VS(k_sector0, "sales_k_nominal");

	 		v[31] = VS(k_sector0, "cost_workers_k");
	 		v[32] = VS(k_sector0, "research_cost_k") - VS(government0, "green_research_subsidie_cost");
	 		v[33] = VS(k_sector0, "profits_distributed_k");
	 		v[34] = VS(k_sector0, "taxes_sales_paid_k");
	 		v[35] = VS(k_sector0, "deposits_k") - VLS(k_sector0, "deposits_k", 1);
			v[36] = VS(k_sector0, "cost_managers_k");
			v[37] = VS(k_sector0, "emissions_k") * VS(government0, "CO2_tax");
	 		v[30] = v[31] + v[32] + v[33] + v[34] + v[35] + v[36];

			v[40] = v[20] - v[30];
			
			v[10] = v[20] == 0? 1: ( v[20] - v[30] ) / v[20]; // should be equal to zero

			if( v[20] == 0 && v[30] == 0 ) v[10] = 0; // correct problems in case of division by zero
			
			v[11] = abs(v[10] );

			if(v[11]>=V("check_threshold"))
			{	
				//INTERACT("check_flows_firm_k", v[11]);

				LOG("\n -> WARNING: error in check K firm flows equal to  %.2lf in time %.0lf", v[11], T);

				
			}

			 

	 	RESULT(v[11])


		EQUATION("_check_flows_firm_k")
			/* UNIT: percentage points

			Checks the consistency of K firm's flows 

			organization:
				v[20] - v[29] > source of funds
				v[31] - v[39] > use of funds
			*/

			VS(GRANDPARENT, "order_equations"); // Simulations in current period must have finished.

	 		v[20] =  V( "_sales_k_nominal");
			v[20] += V( "_loan_k") - VL( "_loan_k", 1);

	 		v[31] =  V( "_cost_production_k");
	 		v[32] =  V( "_research_cost_k") - VS(government2, "green_research_subsidie");
	 		v[33] =  V( "_profits_distributed_k");
	 		v[34] =  V( "_taxes_sales_paid_k");
	 		v[35] =  V( "_deposits_k") -  VL( "_deposits_k", 1);
			v[36] =  V( "_loan_k") * VLS(financial2, "interest_rate_loan", 1);
	 		v[30] = v[31] + v[32] + v[33] + v[34] + v[35] + v[36];

			v[40] = v[20] - v[30];
			
			v[10] = v[20] == 0? 1 : ( v[20] - v[30] ) / v[20]; // should be equal to zero

			if( v[20] == 0 && ROUND( v[30], 0, 0.001 ) == 0 ) v[10] = 0; // correct problems in case of division by zero
			
			v[11] = abs(v[10] );

			if(v[11] >= V("check_threshold"))
			{	
				//INTERACT("_check_flows_firm_k", v[11]);

				//LOG("\n -> WARNING: error in check K firm flows equal to  %.2lf for firm %.0lf at %.0lf", v[11], V("_id_firm_k"), T );
				
			}

			 

	 	RESULT(v[11])


	 	EQUATION("check_flows_bank")
	 		/* UNIT: percentage points

	 		Checks the consistency of bank's flows 

	 		organization:
				v[20] - v[29] > source of funds
				v[31] - v[39] > use of funds
			*/

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

			v[40] = VS(financial0, "loans_unpaid_exited_firms"); // loans unpaid by firms that exited
			v[41] = VS(financial0, "loans_paid_exited_firms"); // loans paid by firms that exited

	 		v[21] = VLS(financial0, "bank_loans", 1) * VLS(financial0, "interest_rate_loan", 1);
	 		v[22] = VLS(financial0, "bank_gov_bonds", 1) * VLS(financial0, "policy_interest_rate", 1);
	 		v[23] = VS(financial0, "bank_deposits") + v[41] - VLS(financial0, "bank_deposits", 1);
			v[24] = VLS(financial0, "bank_gov_green_bonds", 1) * VLS(financial0, "green_policy_interest_rate", 1);
	 		v[20] = v[21] + v[22] + v[23] + v[24];

	 		v[31] = VS(financial0, "bank_loans") + v[41] + v[40] - VLS(financial0, "bank_loans", 1);
	 		v[32] = VS(financial0, "bank_gov_bonds") - VLS(financial0, "bank_gov_bonds", 1);
			v[33] = VS(financial0, "bank_gov_green_bonds") - VLS(financial0, "bank_gov_green_bonds", 1);
			v[30] = v[31] + v[32] + v[33] ;

			if( v[20] == 0 && v[30] == 0 ) END_EQUATION( 0 ); 

			v[9] = v[20] - v[30];

			v[10] = v[20] == 0 ?  1 : ( v[20] - v[30] ) / v[20]; // should be equal to zero
			
			v[11] = abs(  v[10] );

			if(v[11] >= V("check_threshold"))
			{	
				//INTERACT("flow_bank", v[10]);

				LOG("\n -> WARNING: error in check bank flows equal to  %.2lf in time %.0lf", v[10], T);
			}

	 	RESULT( v[11] )

	 	EQUATION("check_flows_government")
	 		/* UNIT: percentage points

	 		Checks the consistency of government's flows (without considering the CB)

	 		organization:
				v[20] - v[29] > source of funds
				v[31] - v[39] > use of funds
			*/

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

	 		v[21] = VS(government0, "taxes_revenue");
	 		v[22] = VS(government0, "bonds_flow");
			v[23] = VS(government0, "taxes_emissions") + VS(hh_sector0, "dividends_taxes") + VS(hh_sector0, "wealth_tax");
	 		v[20] = v[21] + v[22] + v[23];

	 		v[31] = VS(government0, "unemployment_benefit_paid") + VS(government0, "consumption_realized_gov"); 
	 		v[32] = VS(government0, "bonds_interest_paid") ;
	 		v[33] = VS(government0, "gov_deposits_cb") - VLS(government0, "gov_deposits_cb", 1);
			v[34] = VS(government0, "green_government_spending") ;
	 		v[30] = v[31] + v[32] + v[33] + v[34];
			
			v[10] = ( v[20] - v[30] ) / v[20]; // should be equal to zero
			
			v[11] = abs( v[10] );

			if(v[11]>=V("check_threshold"))
			{
				LOG("\n -> WARNING: error in check government flows equal to  %.2lf in time %.0lf", v[11], T);
			}

	 	RESULT( v[11] )

	 		
	 // 1b. HORIZONTAL CHECKS

	 	EQUATION("check_flows_consumption")
			/* UNIT: percentage points

			Equation that checks whether the total value of realized consumption by HHs from the domestic sector is equal to the total of sales of domestic firms to HHs (aggregate values)  */

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

			v[0] = VS(hh_sector0, "consumption_realized_hh") + VS(government0, "consumption_realized_gov") ; // counter of aggregate consumption from domestic production
			v[1] = VS(c_sector0, "sales_c_nominal") ; // counter of aggregate sales to the domestic sector

			v[2] = v[0] == 0 ? 1: (v[1] - v[0]) / v[0]; // value should be equal to zero

			v[3] = abs( v[2] ); 

			if(v[3] >= V("check_threshold"))
			{
				LOG("\n -> WARNING: error in check consumption flow equal to  %.2lf in time %.0lf", v[3], T);
			}

		RESULT(v[3])

		EQUATION("check_flows_wages")
			/* UNIT: Percentage points

			Checks whether total of wages paid is equal to total of wages received */

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

			v[0] = VS(hh_sector0, "wage_bill");

			v[1] = VS(c_sector0, "cost_workers_c");
			v[1] += VS(k_sector0, "cost_workers_k");
			v[1] += VS(k_sector0, "research_cost_k");
			v[1] += VS(c_sector0, "cost_managers_c");
			v[1] += VS(k_sector0, "cost_managers_k");

			v[10] = (v[0] - v[1]) / v[0];

			v[11] = abs( v[10] );

			if(v[11]>=V("check_threshold"))
			{
				LOG("\n -> WARNING: error in check wage flows equal to %.2lf in time %.0lf", v[11], T);
			}

		RESULT(v[11])

		EQUATION("check_flows_profits")
			/* UNIT: Percentage points

			Checks whether total of profits distributed is equal to total of profits received as dividends */

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

			v[0] = VS(hh_sector0, "dividends"); // sum of profit dividends received by the households

			v[1] = VLS(c_sector0, "profits_distributed_c", 1); 
			v[2] = VS(k_sector0, "profits_distributed_k"); 
			v[3] = v[1] + v[2];

			if(v[0]!=0)
			{
				v[10] = ( v[3] - v[0] ) / v[0]; // difference should be equal to zero			
			}
			else
			{
				if(v[3] == 0) // if profits distributed are also zero;
				{
					v[10] = 0;
				}
				else
				{
					v[10] = (v[3] - v[0]) / v[3];
				}
			}
	
			v[11] = abs( v[10] ); 

			if(v[11] >= V("check_threshold"))
			{
				LOG("\n -> WARNING: error in check profits distributed equal to  %.2lf in time %.0lf", v[11], T);
			}

		RESULT(v[11])

		EQUATION("check_flows_deposits")
			/*  UNIT: percentage points

			Checks whether total change in deposits is equal to change in deposits held at bank */

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

			v[0] = VS(c_sector0, "deposits_c") - VLS(c_sector0, "deposits_c",1);
	 		v[1] = VS(hh_sector0, "deposits_hh") - SUMLS(hh_sector0, "deposits_hh",1);
	 		v[2] = VS(k_sector0, "deposits_k") - VLS(k_sector0, "deposits_k", 1);
	 		v[4] = v[0] + v[1] + v[2] ; // change in deposits from firm and HH sides

	 		v[5] = VS(financial0, "bank_deposits");
	 		v[6] = VLS(financial0, "bank_deposits",1);
	 		v[7] = v[5] - v[6]; // change in deposits from bank side

	 		if(v[7]!=0)
	 		{
	 			v[8] = (v[7] - v[4])/v[7];
	 		}
	 		else
	 		{
	 			if(v[4] == 0)
	 			{
	 				v[8] = 0;
	 			}
	 			else
	 			{
	 				v[8] = (v[4] - v[7]) / v[4];
	 			}
	 		}

	 		v[9] = abs( v[8] );

	 		if(v[9]>=V("check_threshold"))
	 		{
	 			LOG("\n -> WARNING: error in check deposits flow equal to %.2lf in time %.0lf", v[9], T);
	 		}

	 	RESULT(v[9])

	 	EQUATION("check_flows_loan")
	 		/* UNIT: percentage points

	 		Checks whether total change in loans is equal to total change in loans held at the bank */

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

			v[0] = VS(c_sector0, "loan_c") + VS(k_sector0, "loan_k");
	 		v[1] = VLS(c_sector0, "loan_c", 1) + VLS(k_sector0, "loan_k", 1);
			v[2] = v[0] - v[1]; // change in debt

			v[3] = VS(financial0, "bank_loans");
			v[4] = VLS(financial0, "bank_loans", 1);
			v[5] = v[3] - v[4];

			if(v[2] != 0)
			{
				v[6] = (v[2] - v[5]) / v[2];
			}
			else
			{
				if(v[5] == 0)
				{
					v[6] = 0;
				}
				else
				{
					v[6] = ( v[5] - v[2] ) / v[5];
				}
			}
			
			v[7] = abs( v[6] );

			if(v[7]>=V("check_threshold"))
			{
				LOG("\n -> WARNING: error in check loans flows equal to %.2lf in time %.0lf", v[7], T);
			} 

		RESULT(v[7])




// -----------------------------------------------------------------------------------------------------------------------
// 2. BALANCE SHEET MATRIX CHECKS -----------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

		EQUATION("check_stocks_loan")
			/* UNIT: percentage points

			Checks whether loans stocks match */

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

			v[0] = VS(c_sector0, "loan_c") + VS(k_sector0, "loan_k");
			v[1] = VS(financial0, "bank_loans");

			v[2] = (v[0] - v[1]) / v[0];

			v[3] = abs( v[2] );

			if(v[0] == 0 && v[1] == 0) v[3] = 0;
			
			if(v[3]>=V("check_threshold"))
			{
				LOG("\n -> WARNING: error in check loan stock equal to %.2lf in time %.0lf", v[3], T);
			}
						
		RESULT(v[3])

		EQUATION("check_stocks_deposits")
			/* UNIT: percentage points

			Checks whether deposits stock match */

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

			v[21] = VS(c_sector0, "deposits_c");
			v[22] = VS(hh_sector0, "deposits_hh");
			v[23] = VS(k_sector0, "deposits_k");
			v[20] = v[21] + v[22] + v[23] ;

			v[30] = VS(financial0, "bank_deposits");

			v[3] = (v[20] - v[30]) / v[20];

			v[4] = abs( v[3] );

			if(v[4]>=V("check_threshold"))
			{
				LOG("\n -> WARNING: error in check deposits stock equal to %.2lf in time %.0lf", v[4], T);
			} 

		RESULT(v[4])

		EQUATION("check_stocks_bank")

			VS(PARENT, "order_equations"); // Simulations in current period must have finished.

			v[21] = VS(financial0, "bank_deposits");
			v[22] = VS(financial0, "bank_net_worth");
			v[20] = v[21] + v[22];

			v[31] = VS(financial0, "bank_loans");	
			v[32] = VS(financial0, "bank_gov_bonds");
			v[33] = VS(financial0, "bank_reserves_cb");
			v[34] = VS(financial0, "bank_gov_green_bonds");
			v[30] = v[31] + v[32] + v[33];

			v[10] = (v[20] - v[30]) / v[20];

			v[11] = abs( v[10] );

			if(v[11]>=V("check_threshold"))
			{
				LOG("\n -> WARNING: error in check bank stock equal to %.2lf in time %.0lf", v[11], T);
			} 

		RESULT(v[11])

// -----------------------------------------------------------------------------------------------------------------------
// 3. OTHER CHECKS -----------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------


	EQUATION("check_sales")
		/* UNIT: percentage points 

		Equation that checks whether the firm's production is equal to its sales and change in inventories (in real terms) -> if 0: ok, else: there is a problem! */

		VS(PARENT, "order_equations"); // Simulations in current period must have finished.

		v[21] = VS(c_sector0, "sales_c_real"); // firm's sales in real terms
		v[22] = VS(c_sector0, "inventories_c_real") + VS(c_sector0, "exit_inventories_destroyed") - VLS(c_sector0, "inventories_c_real", 1);
		v[20] = v[21] + v[22];

		v[30] = VS(c_sector0, "production_c_real"); // firm's production in real terms

		v[10] = (v[20] - v[30]) / v[20];

		v[11] = abs( v[10] );

		if(v[11]>=V("check_threshold"))
		{
			LOG("\n -> WARNING: error in check sales (real) equal to  %.2lf in time %.0lf", v[11], T);
		} 

	RESULT(v[11])


	EQUATION("check_macro_aggregates")
		/* UNIT: percentage points

		Error in macro aggregates -> equality between demand and expenditure approaches */

		VS(PARENT, "order_equations"); // Simulations in current period must have finished.

		// gdp from income approach
			v[0] = V( "gdp_nominal");

		// gdp from demand approach
			v[1] = V( "gdp_nominal_demand_approach");

		// difference between different measures

			v[2] = v[0] - v[1];

			v[6] = (v[2] / v[0]); // difference should be equal to zero

			v[7] = abs( v[6] ); 

			if(v[7]>=V("check_threshold"))
			{
				LOG("\n -> WARNING: error in nominal macro aggregates equal to  %.2lf in time %.0lf", v[7], T);
			}

	RESULT(v[7])

	EQUATION("check_macro_aggregates_real")
		/* UNIT: percentage points

		Error in macro aggregates -> compares real production with real expenditure */

		VS(PARENT, "order_equations"); // Simulations in current period must have finished.

		v[0] = V( "gdp_real"); // real gdp from production approach

		v[1] = V( "gdp_real_demand_approach"); // real gdp from demand approach

		v[10] = v[0] - v[1];

		v[11] = (v[10] / v[0]);

		v[12] = abs( v[11] );

		if(v[12]>=V("check_threshold"))
			{
				LOG( "\n -> WARNING: error in macro real aggregates check equal to  %.2lf in time %.0lf", v[12], T );
			}

	RESULT( v[12] )
