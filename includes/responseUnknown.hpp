/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUnknown.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnorazma <nnorazma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 14:39:25 by nnorazma          #+#    #+#             */
/*   Updated: 2023/09/12 15:01:59 by nnorazma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"
#include "responseBase.hpp"

class ResponseUnknown : public ResponseBase {
	private:

	public:
		ResponseUnknown( void );
		~ResponseUnknown( void );

		void generateResponse( void );

};