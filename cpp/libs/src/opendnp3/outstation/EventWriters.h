/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */

#ifndef OPENDNP3_EVENTWRITERS_H
#define OPENDNP3_EVENTWRITERS_H

#include "openpal/serialization/Serialization.h"
#include "opendnp3/outstation/EventWriteHandler.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/HeaderWriter.h"

namespace opendnp3
{
	

	template <class T>
	class PrefixedEventWriter final : public EventWriter<T>
	{
		PrefixedWriteIterator<openpal::UInt16, T> iterator;		

	public:		

		PrefixedEventWriter(HeaderWriter& writer, const DNP3Serializer<T>& serializer) :
			iterator(
				writer.IterateOverCountWithPrefix<openpal::UInt16, T>(QualifierCode::UINT16_CNT_UINT16_INDEX, serializer)
			)
		{

		}

		virtual bool Write(const T& meas, uint16_t index) override {
			return iterator.IsValid() ? iterator.Write(meas, index) : false;
		}
	};

	class EventWriters
	{

	public:

		template <class T>
		static uint16_t WritePrefixed(HeaderWriter& writer, EventCollection<T>& items, const DNP3Serializer<T>& serializer)
		{
			PrefixedEventWriter<T> handler(writer, serializer);
			return items.WriteSome(handler);
		}
	};

}

#endif